#include "zf_common_headfile.h"
#include "isr_config.h"

#include "ano.h"
#include "balance.h"
#include "attitude.h"
#pragma section all "cpu0_dsram"
// ���������#pragma section all restore���֮���ȫ�ֱ���������CPU0��RAM��


// ���̵��뵽���֮��Ӧ��ѡ�й���Ȼ����refreshˢ��һ��֮���ٱ���
// ����Ĭ������Ϊ�ر��Ż��������Լ��һ�����ѡ��properties->C/C++ Build->Setting
// Ȼ�����Ҳ�Ĵ������ҵ�C/C++ Compiler->Optimization->Optimization level�������Ż��ȼ�
// һ��Ĭ���½����Ĺ��̶���Ĭ�Ͽ�2���Ż�����˴��Ҳ��������Ϊ2���Ż�

// ����TCϵ��Ĭ���ǲ�֧���ж�Ƕ�׵ģ�ϣ��֧���ж�Ƕ����Ҫ���ж���ʹ�� interrupt_global_enable(0); �������ж�Ƕ��
// �򵥵�˵ʵ���Ͻ����жϺ�TCϵ�е�Ӳ���Զ������� interrupt_global_disable(); ���ܾ���Ӧ�κε��жϣ������Ҫ�����Լ��ֶ����� interrupt_global_enable(0); �������жϵ���Ӧ��

// *************************** ����Ӳ������˵�� ***************************
// ʹ����ɿƼ� Ӣ����TriCore ��������������
//      ֱ�ӽ���������ȷ�����ں��İ�ĵ������ؽӿڼ���
// ʹ�� USB-TTL ģ������
//      ģ��ܽ�            ��Ƭ���ܽ�
//      USB-TTL-RX          �鿴 zf_common_debug.h �ļ��� DEBUG_UART_TX_PIN �궨������� Ĭ�� P14_0
//      USB-TTL-TX          �鿴 zf_common_debug.h �ļ��� DEBUG_UART_RX_PIN �궨������� Ĭ�� P14_1
//      USB-TTL-GND         ���İ��Դ�� GND
//      USB-TTL-3V3         ���İ� 3V3 ��Դ
// ���� MPU6050
//      ģ��ܽ�            ��Ƭ���ܽ�
//      SCL/SPC             �鿴 zf_device_imu660ra.h �� IMU660RA_SPC_PIN �궨�� Ĭ�� P20_11
//      SDA/DSI             �鿴 zf_device_imu660ra.h �� IMU660RA_SDI_PIN �궨�� Ĭ�� P20_14
//      SA0/SDO             �鿴 zf_device_imu660ra.h �� IMU660RA_SDO_PIN �궨�� Ĭ�� P20_12
//      CS                  �鿴 zf_device_imu660ra.h �� IMU660RA_CS_PIN  �궨�� Ĭ�� P20_13
//      GND                 ��Դ�� GND
//      3V3                 ��Դ 3V3



// *************************** ���̲���˵�� ***************************
// 1.���İ���¼��ɱ����̣�����ʹ�ú��İ���������������� USB-TTL ģ�飬�����Ӻñ��������ڶϵ�������������
// 2.���������������� USB-TTL ģ�����ӵ��� ����ϵ� ���� H2 LED ����˸
// 3.������ʹ�ô������ִ򿪶�Ӧ�Ĵ��ڣ����ڲ�����Ϊ zf_common_debug.h �ļ��� DEBUG_UART_BAUDRATE �궨�� Ĭ�� 115200�����İ尴�¸�λ����
// 4.�����ڴ��������Ͽ������´�����Ϣ��
//      IMU660RA acc data: x-..., y-..., z-...
//      IMU660RA gyro data: x-..., y-..., z-...
// 5.�ƶ���ת IMU660RA �ͻῴ����ֵ�仯
// �������������˵�����ز��� ����ձ��ļ����·� ���̳�������˵�� �����Ų�


// **************************** �������� ****************************
#define LED1                    (P20_9)




//����
#define FLYWHEEL_CLOCKWISE              (1                      )     // ������˳ʱ����ת����
#define FLYWHEEL_DUTY_LEVEL             (0                      )     // ������ռ�ձ���Ч��ƽ

#define LEFT_FLYWHEEL_PWM               (ATOM0_CH7_P02_7        )     // ������PWM��������
#define LEFT_FLYWHEEL_DIR               (P02_6                  )     // �����ַ����������
#define LEFT_FLYWHEEL_BRAKE             (P11_3                  )     // ������ɲ����������
#define LEFT_FLYWHEEL_ENCODER_INDEX     (TIM5_ENCODER          )     // �����ֱ��������
#define LEFT_FLYWHEEL_ENCODER_CH1       (TIM5_ENCODER_CH1_P10_3)     // �����ֱ�����ͨ��1
#define LEFT_FLYWHEEL_ENCODER_CH2       (TIM5_ENCODER_CH2_P10_1)     // �����ֱ�����ͨ��2

#define RIGHT_FLYWHEEL_PWM              (ATOM0_CH5_P02_5        )     // �Ҳ����PWM��������
#define RIGHT_FLYWHEEL_DIR              (P02_4                  )     // �Ҳ���ַ����������
#define RIGHT_FLYWHEEL_BRAKE            (P11_2                  )     // �Ҳ����ɲ����������
#define RIGHT_FLYWHEEL_ENCODER_INDEX    (TIM2_ENCODER          )     // �Ҳ���ֱ��������
#define RIGHT_FLYWHEEL_ENCODER_CH1      (TIM2_ENCODER_CH1_P33_7)     // �Ҳ���ֱ�����ͨ��1
#define RIGHT_FLYWHEEL_ENCODER_CH2      (TIM2_ENCODER_CH2_P33_6)     // �Ҳ���ֱ�����ͨ��2



//��ص�ѹ
#define VOLTAGE_PORT    ADC0_CH8_A8 // ������˿�



int flag_50ms=0;


int core0_main(void)
{
    clock_init();                   // ��ȡʱ��Ƶ��<��ر���>
    debug_init();                   // ��ʼ��Ĭ�ϵ��Դ���
    // �˴���д�û����� ���������ʼ�������


    //��ص�ѹ
    uint16 valtage_adc = 0;
    float valtage_now = 0;
    adc_init(VOLTAGE_PORT, ADC_12BIT); // ��ؼ�� ADC2

    key_init(100);
    encoder_quad_init(LEFT_FLYWHEEL_ENCODER_INDEX, LEFT_FLYWHEEL_ENCODER_CH1, LEFT_FLYWHEEL_ENCODER_CH2);   // ��ʼ�������ֱ������ӿ�
    encoder_quad_init(RIGHT_FLYWHEEL_ENCODER_INDEX, RIGHT_FLYWHEEL_ENCODER_CH1, RIGHT_FLYWHEEL_ENCODER_CH2);// ��ʼ���Ҳ���ֱ������ӿ�


    //LED��ʼ��
    gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL);                             // ��ʼ�� LED1 ��� Ĭ�ϸߵ�ƽ �������ģʽ

    //��Ļ��ʼ��--------------------------------------


//--------------------------�����ǳ�ʼ��-----------------------------------------------------------------------------------------------
    while(1)
    {
       if(imu660ra_init())
           printf("\r\n IMU660RA init error.");  // IMU660RA ��ʼ��ʧ��
       else
           break;
       gpio_toggle_level(LED1);    // ��ת LED ���������ƽ
       system_delay_ms(300);
    }


//
//    while(1)
//       {
//           if(dl1a_init())
//               gpio_toggle_level(LED1);                                            // ��ת LED ���������ƽ ���� LED ���� ��ʼ����������ƻ����ĺ���
//           else
//               break;
//           system_delay_ms(1000);                                                  // ���Ʊ�ʾ�쳣
//       }

//--------------------------�����ǳ�ʼ��end------------------------------------------------------------------------------------------------

    FlyWheelinit();


    cpu_wait_event_ready();         // �ȴ����к��ĳ�ʼ�����


    pit_ms_init(CCU60_CH0,5); //ÿ5������̬����

    pit_ms_init(CCU60_CH1,1); //ÿ5������̬����

    pit_ms_init(CCU61_CH0,100); //ÿ100���붨ʱ

    int16 SendData[3]= {0};



    Balance_init();
                                                     // PWM ͨ����ʼ��Ƶ�� 17KHz ռ�ձȳ�ʼΪ 0

    while(1){


        system_delay_ms(2);
    }


  //  PID_init();



//    pit_ms_init(CCU60_CH1,5);
    //ѭ����ʼ

    //Balance_init();

    return 0;
}




IFX_INTERRUPT(cc60_pit_ch1_isr, 0, CCU6_0_CH1_ISR_PRIORITY)
{


    interrupt_global_enable(0);                     // ״̬��ʾ
    pit_clear_flag(CCU60_CH1);

    balance();
}

IFX_INTERRUPT(cc60_pit_ch0_isr, 0, CCU6_0_CH0_ISR_PRIORITY)
{
    //ÿ5����һ��,��̬����.
    interrupt_global_enable(0);                     // �����ж�Ƕ��
    pit_clear_flag(CCU60_CH0);

    flag_50ms+=1;

    ICM_getEulerianAngles();


}
IFX_INTERRUPT(cc61_pit_ch0_isr, 0, CCU6_1_CH0_ISR_PRIORITY)
{
    interrupt_global_enable(0);                     // �����ж�Ƕ��
    pit_clear_flag(CCU61_CH0);

    speed_C();


}

#pragma section all restore
