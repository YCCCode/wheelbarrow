#include "zf_common_headfile.h"
#include "isr_config.h"

#include "ano.h"
#include "balance.h"
#include "attitude.h"
#pragma section all "cpu0_dsram"
// 将本语句与#pragma section all restore语句之间的全局变量都放在CPU0的RAM中


// 工程导入到软件之后，应该选中工程然后点击refresh刷新一下之后再编译
// 工程默认设置为关闭优化，可以自己右击工程选择properties->C/C++ Build->Setting
// 然后在右侧的窗口中找到C/C++ Compiler->Optimization->Optimization level处设置优化等级
// 一般默认新建立的工程都会默认开2级优化，因此大家也可以设置为2级优化

// 对于TC系列默认是不支持中断嵌套的，希望支持中断嵌套需要在中断内使用 interrupt_global_enable(0); 来开启中断嵌套
// 简单点说实际上进入中断后TC系列的硬件自动调用了 interrupt_global_disable(); 来拒绝响应任何的中断，因此需要我们自己手动调用 interrupt_global_enable(0); 来开启中断的响应。

// *************************** 例程硬件连接说明 ***************************
// 使用逐飞科技 英飞凌TriCore 调试下载器连接
//      直接将下载器正确连接在核心板的调试下载接口即可
// 使用 USB-TTL 模块连接
//      模块管脚            单片机管脚
//      USB-TTL-RX          查看 zf_common_debug.h 文件中 DEBUG_UART_TX_PIN 宏定义的引脚 默认 P14_0
//      USB-TTL-TX          查看 zf_common_debug.h 文件中 DEBUG_UART_RX_PIN 宏定义的引脚 默认 P14_1
//      USB-TTL-GND         核心板电源地 GND
//      USB-TTL-3V3         核心板 3V3 电源
// 接入 MPU6050
//      模块管脚            单片机管脚
//      SCL/SPC             查看 zf_device_imu660ra.h 中 IMU660RA_SPC_PIN 宏定义 默认 P20_11
//      SDA/DSI             查看 zf_device_imu660ra.h 中 IMU660RA_SDI_PIN 宏定义 默认 P20_14
//      SA0/SDO             查看 zf_device_imu660ra.h 中 IMU660RA_SDO_PIN 宏定义 默认 P20_12
//      CS                  查看 zf_device_imu660ra.h 中 IMU660RA_CS_PIN  宏定义 默认 P20_13
//      GND                 电源地 GND
//      3V3                 电源 3V3



// *************************** 例程测试说明 ***************************
// 1.核心板烧录完成本例程，单独使用核心板与调试下载器或者 USB-TTL 模块，并连接好编码器，在断电情况下完成连接
// 2.将调试下载器或者 USB-TTL 模块连接电脑 完成上电 正常 H2 LED 会闪烁
// 3.电脑上使用串口助手打开对应的串口，串口波特率为 zf_common_debug.h 文件中 DEBUG_UART_BAUDRATE 宏定义 默认 115200，核心板按下复位按键
// 4.可以在串口助手上看到如下串口信息：
//      IMU660RA acc data: x-..., y-..., z-...
//      IMU660RA gyro data: x-..., y-..., z-...
// 5.移动旋转 IMU660RA 就会看到数值变化
// 如果发现现象与说明严重不符 请参照本文件最下方 例程常见问题说明 进行排查


// **************************** 代码区域 ****************************
#define LED1                    (P20_9)




//飞轮
#define FLYWHEEL_CLOCKWISE              (1                      )     // 左侧飞轮顺时针旋转方向
#define FLYWHEEL_DUTY_LEVEL             (0                      )     // 左侧飞轮占空比有效电平

#define LEFT_FLYWHEEL_PWM               (ATOM0_CH7_P02_7        )     // 左侧飞轮PWM控制引脚
#define LEFT_FLYWHEEL_DIR               (P02_6                  )     // 左侧飞轮方向控制引脚
#define LEFT_FLYWHEEL_BRAKE             (P11_3                  )     // 左侧飞轮刹车控制引脚
#define LEFT_FLYWHEEL_ENCODER_INDEX     (TIM5_ENCODER          )     // 左侧飞轮编码器编号
#define LEFT_FLYWHEEL_ENCODER_CH1       (TIM5_ENCODER_CH1_P10_3)     // 左侧飞轮编码器通道1
#define LEFT_FLYWHEEL_ENCODER_CH2       (TIM5_ENCODER_CH2_P10_1)     // 左侧飞轮编码器通道2

#define RIGHT_FLYWHEEL_PWM              (ATOM0_CH5_P02_5        )     // 右侧飞轮PWM控制引脚
#define RIGHT_FLYWHEEL_DIR              (P02_4                  )     // 右侧飞轮方向控制引脚
#define RIGHT_FLYWHEEL_BRAKE            (P11_2                  )     // 右侧飞轮刹车控制引脚
#define RIGHT_FLYWHEEL_ENCODER_INDEX    (TIM2_ENCODER          )     // 右侧飞轮编码器编号
#define RIGHT_FLYWHEEL_ENCODER_CH1      (TIM2_ENCODER_CH1_P33_7)     // 右侧飞轮编码器通道1
#define RIGHT_FLYWHEEL_ENCODER_CH2      (TIM2_ENCODER_CH2_P33_6)     // 右侧飞轮编码器通道2



//电池电压
#define VOLTAGE_PORT    ADC0_CH8_A8 // 定义检测端口



int flag_50ms=0;


int core0_main(void)
{
    clock_init();                   // 获取时钟频率<务必保留>
    debug_init();                   // 初始化默认调试串口
    // 此处编写用户代码 例如外设初始化代码等


    //电池电压
    uint16 valtage_adc = 0;
    float valtage_now = 0;
    adc_init(VOLTAGE_PORT, ADC_12BIT); // 电池检测 ADC2

    key_init(100);
    encoder_quad_init(LEFT_FLYWHEEL_ENCODER_INDEX, LEFT_FLYWHEEL_ENCODER_CH1, LEFT_FLYWHEEL_ENCODER_CH2);   // 初始化左侧飞轮编码器接口
    encoder_quad_init(RIGHT_FLYWHEEL_ENCODER_INDEX, RIGHT_FLYWHEEL_ENCODER_CH1, RIGHT_FLYWHEEL_ENCODER_CH2);// 初始化右侧飞轮编码器接口


    //LED初始化
    gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL);                             // 初始化 LED1 输出 默认高电平 推挽输出模式

    //屏幕初始化--------------------------------------


//--------------------------陀螺仪初始化-----------------------------------------------------------------------------------------------
    while(1)
    {
       if(imu660ra_init())
           printf("\r\n IMU660RA init error.");  // IMU660RA 初始化失败
       else
           break;
       gpio_toggle_level(LED1);    // 翻转 LED 引脚输出电平
       system_delay_ms(300);
    }


//
//    while(1)
//       {
//           if(dl1a_init())
//               gpio_toggle_level(LED1);                                            // 翻转 LED 引脚输出电平 控制 LED 亮灭 初始化出错这个灯会闪的很慢
//           else
//               break;
//           system_delay_ms(1000);                                                  // 闪灯表示异常
//       }

//--------------------------陀螺仪初始化end------------------------------------------------------------------------------------------------

    FlyWheelinit();


    cpu_wait_event_ready();         // 等待所有核心初始化完毕


    pit_ms_init(CCU60_CH0,5); //每5毫秒姿态解算

    pit_ms_init(CCU60_CH1,1); //每5毫秒姿态解算

    pit_ms_init(CCU61_CH0,100); //每100毫秒定时

    int16 SendData[3]= {0};



    Balance_init();
                                                     // PWM 通道初始化频率 17KHz 占空比初始为 0

    while(1){


        system_delay_ms(2);
    }


  //  PID_init();



//    pit_ms_init(CCU60_CH1,5);
    //循环开始

    //Balance_init();

    return 0;
}




IFX_INTERRUPT(cc60_pit_ch1_isr, 0, CCU6_0_CH1_ISR_PRIORITY)
{


    interrupt_global_enable(0);                     // 状态显示
    pit_clear_flag(CCU60_CH1);

    balance();
}

IFX_INTERRUPT(cc60_pit_ch0_isr, 0, CCU6_0_CH0_ISR_PRIORITY)
{
    //每5毫秒一次,姿态解算.
    interrupt_global_enable(0);                     // 开启中断嵌套
    pit_clear_flag(CCU60_CH0);

    flag_50ms+=1;

    ICM_getEulerianAngles();


}
IFX_INTERRUPT(cc61_pit_ch0_isr, 0, CCU6_1_CH0_ISR_PRIORITY)
{
    interrupt_global_enable(0);                     // 开启中断嵌套
    pit_clear_flag(CCU61_CH0);

    speed_C();


}

#pragma section all restore
