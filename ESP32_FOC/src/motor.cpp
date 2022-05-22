/*
 * @Descripttion: 
 * @version: 
 * @Author: congsir
 * @Date: 2022-05-22 05:30:09
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-05-22 17:42:34
 */
#include <motor.h>
#include <main.h>
#include <SimpleFOC.h>

struct _motor_message *send_message;

//死区制动百分率
static const float DEAD_ZONE_DETENT_PERCENT = 0.2;
//死区RAD?  
static const float DEAD_ZONE_RAD = 1 * _PI / 180;

//怠速速度ewma alpha
static const float IDLE_VELOCITY_EWMA_ALPHA = 0.001;
//怠速速度每秒钟弧度
static const float IDLE_VELOCITY_RAD_PER_SEC = 0.05;
//怠速修正延迟millis
static const uint32_t IDLE_CORRECTION_DELAY_MILLIS = 500;
//怠速校正最大角度rad
static const float IDLE_CORRECTION_MAX_ANGLE_RAD = 5 * PI / 180;
//怠速修正率
static const float IDLE_CORRECTION_RATE_ALPHA = 0.0005;

//先设置一个默认值，等消息队列发送状态过来
KnobConfig config = {
    .num_positions = 32,
    .position = 0,
    .position_width_radians = 8.225806452 * _PI / 180,
    .detent_strength_unit = 2,
    .endstop_strength_unit = 1,
    .snap_point = 1,
};





//设置位置传感器（编码器、磁传感器、霍尔传感器）
// MagneticSensorI2C sensor = MagneticSensorI2C(0x36, 12, 0x0E, 4);
MagneticSensorI2C sensor = MagneticSensorI2C(AS5600_I2C);
TwoWire I2Cone = TwoWire(0);
//设置电机(极对数)
BLDCMotor motor = BLDCMotor(7);
//设置驱动器 （无刷直流电机、步进电机）
// BLDCDriver3PWM( pin_pwmA, pin_pwmB, pin_pwmC, enable使能引脚（可选的）)
//BLDCDriver3PWM driver = BLDCDriver3PWM(26, 27, 14, 12);
BLDCDriver3PWM driver = BLDCDriver3PWM(22, 4, 5, 21);

//设置电流检测 (如果不支持、可以跳过)
//在线电流检测 InlineCurrentSense。低侧电流检测 LowsideCurrentSense。
// InlineCurrentSense current_sense = InlineCurrentSense(0.01, 50, A0, A2);

//设置控制命令接口
Commander command = Commander(Serial);
void onMotor(char *cmd) { command.motor(&motor, cmd); }

//当前相对位置
float current_detent_center = 0;
//上次空闲开始状态
uint32_t last_idle_start = 0;
//怠速检查速度
float idle_check_velocity_ewma = 0;

// LED 闪烁
void Blink(int n)
{
    for (int i = 0; i < n; i++)
    {
        digitalWrite(LED_PIN, HIGH);
        _delay(200);
        digitalWrite(LED_PIN, LOW);
        _delay(100);
    }
}

float CLAMP(const float value, const float low, const float high) 
{
    return value < low ? low : (value > high ? high : value); 
}




void Task_foc(void *pvParameters)
{
    (void)pvParameters;

    send_message = &MOTOR_MSG;
    send_message->ucMessageID = 0;
    xQueueSend(motor_msg_Queue, &send_message, (TickType_t)0);

    //与I2C接线有关  设置频率
    I2Cone.begin(19, 18, 400000UL);
    //初始化传感器
    sensor.init(&I2Cone);
    //传感器与电机连接
    motor.linkSensor(&sensor);
    //设置驱动器电压
    driver.voltage_power_supply = 12;
    //驱动器初始化
    driver.init();
    //驱动器与电机连接
    motor.linkDriver(&driver);
    //传感器与电机校准参数
    motor.voltage_sensor_align = 3;
    //设置FOC调制类型
    // motor.foc_modulation = FOCModulationType::SpaceVectorPWM;
    //设置控制环类型：
    motor.controller = MotionControlType::torque;

    //控制环 PID控制器
    motor.PID_velocity.P = 0.085;
    motor.PID_velocity.I = 1.5;
    motor.PID_velocity.D = 0.0002;
    motor.LPF_velocity.Tf = 0.02; //速度低通滤波
    motor.P_angle.P = 20;         //角度P控制器
    motor.velocity_limit = 50;    //位置控制的最大速度

    //电机参数监控输出串口
    motor.useMonitoring(Serial);
    //电机初始化
    motor.init();
    //校准编码器、启用FOC
    motor.initFOC();
    
    
    send_message = &MOTOR_MSG;
    send_message->ucMessageID = 1;
    xQueueSend(motor_msg_Queue, &send_message, (TickType_t)0);

    pinMode(LED_PIN, OUTPUT);
    Blink(3);

    //定义电机ID
    command.add('M', onMotor, (char *)"motor");

    // 设置目标电压
    float target_voltage = 2;
    float strength = 5;
    motor.move(strength);
    for (int i = 0; i < 3; i++)
    {
        motor.loopFOC();
        delay(1);
    }
    motor.move(-strength);
    for (int i = 0; i < 3; i++)
    {
        motor.loopFOC();
        delay(1);
    }
    motor.move(0);
    motor.loopFOC();

    //上次的相对位置
    current_detent_center = motor.shaft_angle;
    motor.PID_velocity.limit = 10;

    const float derivative_lower_strength = 1 * 0.08;
    const float derivative_upper_strength = 1 * 0.02;
    const float derivative_position_width_lower = radians(3);
    const float derivative_position_width_upper = radians(8);
    const float raw = derivative_lower_strength + (derivative_upper_strength - derivative_lower_strength)/(derivative_position_width_upper - derivative_position_width_lower)*(config.position_width_radians - derivative_position_width_lower);
    //CLAMP可以将随机变化的值限制在一个给定的区间[min,max]内
    motor.PID_velocity.D = CLAMP(
        raw,
        min(derivative_lower_strength, derivative_upper_strength),
        max(derivative_lower_strength, derivative_upper_strength)
    );

    send_message = &MOTOR_MSG;
    send_message->ucMessageID = 2;
    xQueueSend(motor_msg_Queue, &send_message, (TickType_t)0);

    for (;;)
    {
    motor.loopFOC();

    idle_check_velocity_ewma = motor.shaft_velocity * IDLE_VELOCITY_EWMA_ALPHA + idle_check_velocity_ewma * (1 - IDLE_VELOCITY_EWMA_ALPHA);
    if (fabsf(idle_check_velocity_ewma) > IDLE_VELOCITY_RAD_PER_SEC) {
        last_idle_start = 0;
    } else {
        if (last_idle_start == 0) {
            last_idle_start = millis();
        }
    }

    // 如果我们没有移动，并且我们接近中心(但不是完全在那里)，慢慢调整中心点以匹配当前位置
    // If we are not moving and we're close to the center (but not exactly there), slowly adjust the centerpoint to match the current position
    if (last_idle_start > 0 && millis() - last_idle_start > IDLE_CORRECTION_DELAY_MILLIS && fabsf(motor.shaft_angle - current_detent_center) < IDLE_CORRECTION_MAX_ANGLE_RAD) {
        current_detent_center = motor.shaft_angle * IDLE_CORRECTION_RATE_ALPHA + current_detent_center * (1 - IDLE_CORRECTION_RATE_ALPHA);
    }

        //到控制中心的角度 差值
    float angle_to_detent_center = motor.shaft_angle - current_detent_center;

    if (angle_to_detent_center > config.position_width_radians * config.snap_point && (config.num_positions <= 0 || config.position > 0)) {
        current_detent_center += config.position_width_radians;
        angle_to_detent_center -= config.position_width_radians;
        config.position--;
    } else if (angle_to_detent_center < -config.position_width_radians * config.snap_point && (config.num_positions <= 0 || config.position < config.num_positions - 1)) {
        current_detent_center -= config.position_width_radians;
        angle_to_detent_center += config.position_width_radians;
        config.position++;
    }

    //CLAMP可以将随机变化的值限制在一个给定的区间[min,max]内
    //死区调整
    float dead_zone_adjustment = CLAMP(
        angle_to_detent_center,
        fmaxf(-config.position_width_radians*DEAD_ZONE_DETENT_PERCENT, -DEAD_ZONE_RAD),
        fminf(config.position_width_radians*DEAD_ZONE_DETENT_PERCENT, DEAD_ZONE_RAD));

    //出界
    bool out_of_bounds = config.num_positions > 0 && ((angle_to_detent_center > 0 && config.position == 0) || (angle_to_detent_center < 0 && config.position == config.num_positions - 1));
    motor.PID_velocity.limit = 10; //out_of_bounds ? 10 : 3;
    motor.PID_velocity.P = out_of_bounds ? config.endstop_strength_unit * 4 : config.detent_strength_unit * 4;

    //处理float类型的取绝对值
    if (fabsf(motor.shaft_velocity) > 60) {
        //如果速度太高 则不增加扭矩
        // Don't apply torque if velocity is too high (helps avoid positive feedback loop/runaway)
        motor.move(0);
    } else {
        float torque = motor.PID_velocity(-angle_to_detent_center + dead_zone_adjustment);
        motor.move(torque);
    }

    //Serial.println(config.position);
    vTaskDelay(2);
        
    }
}