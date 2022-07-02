/*
 * @Descripttion:
 * @version:
 * @Author: congsir
 * @Date: 2022-05-22 05:30:09
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-07-02 11:34:54
 */
#include <motor.h>
#include <main.h>
#include <SimpleFOC.h>
#include <display.h>

static KnobConfig super_knob_configs[] = {
    {
        0,
        0,
        10 * PI / 180,
        0,
        1,
        1.1,
        "Unbounded\nNo detents", //无限制  不制动
    },
    {
        32,
        0,
        8.225806452 * PI / 180,
        2,
        1,
        1.1,
        "Coarse values\nStrong detents", //粗糙的棘轮 强阻尼
    },
    {
        256,
        127,
        1 * PI / 180,
        1,
        1,
        1.1,
        "Fine values\nWith detents", //任意运动的控制  有阻尼 类似于机械旋钮
    },
    {
        256,
        127,
        1 * PI / 180,
        0,
        1,
        1.1,
        "Fine values\nNo detents", //任意运动的控制  无阻尼
    },
    {
        2, //可以运动的个数
        0,
        60 * PI / 180, //每一步60度
        1,             //制动强度为1
        1,
        0.55,                    // Note the snap point is slightly past the midpoint (0.5); compare to normal detents which use a snap point *past* the next value (i.e. > 1)
        "On/off\nStrong detent", //模拟开关  强制动
    },
};

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
KnobConfig motor_config = {
    .num_positions = 32,
    .position = 0,
    .position_width_radians = 8.225806452 * _PI / 180,
    .detent_strength_unit = 2.3,
    .endstop_strength_unit = 1,
    .snap_point = 1.1,
};

//设置位置传感器（编码器、磁传感器、霍尔传感器）
// MagneticSensorI2C sensor = MagneticSensorI2C(0x36, 12, 0x0E, 4);
MagneticSensorI2C sensor = MagneticSensorI2C(AS5600_I2C);
TwoWire I2Cone = TwoWire(0);
//设置电机(极对数)
BLDCMotor motor = BLDCMotor(7);
//设置驱动器 （无刷直流电机、步进电机）
// BLDCDriver3PWM( pin_pwmA, pin_pwmB, pin_pwmC, enable使能引脚（可选的）)
// BLDCDriver3PWM driver = BLDCDriver3PWM(26, 27, 14, 12);
BLDCDriver3PWM driver = BLDCDriver3PWM(22, 4, 5, 21);

//设置电流检测 (如果不支持、可以跳过)
//在线电流检测 InlineCurrentSense。低侧电流检测 LowsideCurrentSense。
// InlineCurrentSense current_sense = InlineCurrentSense(0.01, 50, A0, A2);

//设置控制命令接口 关闭输出 可有效防止抖动
// Commander command = Commander(Serial);
// void onMotor(char *cmd) { command.motor(&motor, cmd); }

// LED 闪烁
void led_blink(int n)
{
    for (int i = 0; i < n; i++)
    {
        digitalWrite(LED_PIN, HIGH);
        vTaskDelay(200);
        digitalWrite(LED_PIN, LOW);
        vTaskDelay(100);
    }
}

//开机初始化角度至0
void init_angle(void)
{
    float target_angle = 0;
    target_angle = sensor.getAngle();
    float delta = volt_limit / init_smooth;
    for (int i = 0; i <= init_smooth; i++)
    {
        motor.voltage_limit = delta * i;
        motor.loopFOC();
        motor.move(target_angle);
    }
    motor.voltage_limit = volt_limit;
}

float CLAMP(const float value, const float low, const float high)
{
    return value < low ? low : (value > high ? high : value);
}

void motor_shake(int strength, int delay_time)
{
    motor.move(strength);
    for (int i = 0; i < delay_time; i++)
    {
        motor.loopFOC();
        vTaskDelay(1);
    }
    motor.move(-strength);
    for (int i = 0; i < delay_time; i++)
    {
        motor.loopFOC();
        vTaskDelay(1);
    }
}

int get_motor_position(void)
{
    return motor_config.position;
}

void update_motor_status(MOTOR_RUNNING_MODE_E motor_status)
{
    struct _knob_message *send_message;
    send_message = &MOTOR_MSG;
    send_message->ucMessageID = motor_status;
    xQueueSend(motor_msg_Queue, &send_message, (TickType_t)0);
}

void update_motor_config(int status)
{
    motor_config = super_knob_configs[status];
}

void Task_foc(void *pvParameters)
{
    (void)pvParameters;

    update_motor_status(MOTOR_INIT);
    led_blink(1);

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
    motor.foc_modulation = FOCModulationType::SpaceVectorPWM;
    //设置控制环类型：先设置为角度控制，方便初次定位
    motor.controller = MotionControlType::angle;

    //控制环 PID控制器
    motor.PID_velocity.P = 0.08;
    motor.PID_velocity.I = 4;
    motor.PID_velocity.D = 0.0002;
    motor.LPF_velocity.Tf = 0.02; //速度低通滤波
    motor.P_angle.P = 20;         //角度P控制器
    motor.velocity_limit = 5;     //位置控制的最大速度

    //电机参数监控输出串口
    // motor.useMonitoring(Serial);
    //电机初始化
    motor.init();
    //校准编码器、启用FOC
    motor.initFOC();
    update_motor_status(MOTOR_INIT_SUCCESS);
    led_blink(1);
    init_angle();
    //设置控制环类型：
    motor.controller = MotionControlType::torque;
    //定义电机ID
    // command.add('M', onMotor, (char *)"motor");
    // motor.move(0);
    motor.loopFOC();

    update_motor_status(MOTOR_INIT_END);
    led_blink(3);

    //当前相对位置
    float current_detent_center = 0;
    //上次空闲开始状态
    uint32_t last_idle_start = 0;
    //怠速检查速度
    float idle_check_velocity_ewma = 0;

    for (;;)
    {
        motor.loopFOC();
        //监听页面状态
        struct _knob_message *lvgl_message;
        if (xQueueReceive(motor_rcv_Queue, &(lvgl_message), (TickType_t)0))
        {
            Serial.print("motor_rcv_Queue --->");
            Serial.println(lvgl_message->ucMessageID);
            switch (lvgl_message->ucMessageID)
            {
            case CHECKOUT_PAGE:
            {
                //上次的相对位置
                current_detent_center = motor.shaft_angle;
                // motor.PID_velocity.limit = 10;

                const float derivative_lower_strength = motor_config.detent_strength_unit * 0.08;
                const float derivative_upper_strength = motor_config.detent_strength_unit * 0.02;
                const float derivative_position_width_lower = radians(3);
                const float derivative_position_width_upper = radians(8);
                const float raw = derivative_lower_strength + (derivative_upper_strength - derivative_lower_strength) / (derivative_position_width_upper - derivative_position_width_lower) * (motor_config.position_width_radians - derivative_position_width_lower);
                // CLAMP可以将随机变化的值限制在一个给定的区间[min,max]内
                motor.PID_velocity.D = CLAMP(
                    raw,
                    min(derivative_lower_strength, derivative_upper_strength),
                    max(derivative_lower_strength, derivative_upper_strength));

                //存在页面切换 就震动一下
                motor_shake(2, 2);
            }
            break;
            case BUTTON_CLICK:
                motor_shake(2, 2);
                break;
            default:
                break;
            }
        }

        idle_check_velocity_ewma = motor.shaft_velocity * IDLE_VELOCITY_EWMA_ALPHA + idle_check_velocity_ewma * (1 - IDLE_VELOCITY_EWMA_ALPHA);
        if (fabsf(idle_check_velocity_ewma) > IDLE_VELOCITY_RAD_PER_SEC)
        {
            last_idle_start = 0;
        }
        else
        {
            if (last_idle_start == 0)
            {
                last_idle_start = millis();
            }
        }

        // 如果我们没有移动，并且我们接近中心(但不是完全在那里)，慢慢调整中心点以匹配当前位置
        // If we are not moving and we're close to the center (but not exactly there), slowly adjust the centerpoint to match the current position
        if (last_idle_start > 0 && millis() - last_idle_start > IDLE_CORRECTION_DELAY_MILLIS && fabsf(motor.shaft_angle - current_detent_center) < IDLE_CORRECTION_MAX_ANGLE_RAD)
        {
            // Serial.println("slowly adjust the centerpoint to match the current position......");
            current_detent_center = motor.shaft_angle * IDLE_CORRECTION_RATE_ALPHA + current_detent_center * (1 - IDLE_CORRECTION_RATE_ALPHA);
        }

        //到控制中心的角度 差值
        float angle_to_detent_center = motor.shaft_angle - current_detent_center;

        if (angle_to_detent_center > motor_config.position_width_radians * motor_config.snap_point && (motor_config.num_positions <= 0 || motor_config.position > 0))
        {
            current_detent_center += motor_config.position_width_radians;
            angle_to_detent_center -= motor_config.position_width_radians;
            motor_config.position--;
        }
        else if (angle_to_detent_center < -motor_config.position_width_radians * motor_config.snap_point && (motor_config.num_positions <= 0 || motor_config.position < motor_config.num_positions - 1))
        {
            current_detent_center -= motor_config.position_width_radians;
            angle_to_detent_center += motor_config.position_width_radians;
            motor_config.position++;
        }

        // CLAMP可以将随机变化的值限制在一个给定的区间[min,max]内
        //死区调整
        float dead_zone_adjustment = CLAMP(
            angle_to_detent_center,
            fmaxf(-motor_config.position_width_radians * DEAD_ZONE_DETENT_PERCENT, -DEAD_ZONE_RAD),
            fminf(motor_config.position_width_radians * DEAD_ZONE_DETENT_PERCENT, DEAD_ZONE_RAD));

        //出界
        bool out_of_bounds = motor_config.num_positions > 0 && ((angle_to_detent_center > 0 && motor_config.position == 0) || (angle_to_detent_center < 0 && motor_config.position == motor_config.num_positions - 1));
        motor.PID_velocity.limit = out_of_bounds ? 10 : 3;
        motor.PID_velocity.P = out_of_bounds ? motor_config.endstop_strength_unit * 4 : motor_config.detent_strength_unit * 4;

        //处理float类型的取绝对值
        if (fabsf(motor.shaft_velocity) > 60)
        {
            //如果速度太高 则不增加扭矩
            // Don't apply torque if velocity is too high (helps avoid positive feedback loop/runaway)
            Serial.println("(motor.shaft_velocity) > 60 !!!");
            motor.move(0);
        }
        else
        {
            float torque = motor.PID_velocity(-angle_to_detent_center + dead_zone_adjustment);
            motor.move(torque);
        }

        // Serial.println(motor_config.position);
        vTaskDelay(1);
    }
}