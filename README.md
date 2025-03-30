# Drive-Stepper-Motor-To-a-Specific-Angle
使用STM32的两个GPIO驱动步进电机转动特定角度，无需占用TIM，但在转动时会堵塞主进程，推荐将MCU作为下位机使用。

StepMotor_PWM_Init()使用TIM驱动步进电机，用来一直转，可控制的变量为PSC，变动PSC以改变转速。

StepMotor_Pulse()的作用是发送一次脉冲，原理就是反转IO口的电平，这点与手动I2C比较相像。

StepMotor_NPulse()与StepMotor_Pulse()内部有两个delay：

Pulse的delay用于决定电平的宽度，参数得看你自己的步进电机，我这里驱动为A4988、电机具体为42BYGH39-401A，delay个150us差不多。如果你的电机在不转动时震动、有电流声，不妨调整一下这个delay。

NPulse的delay用来决定转速，具体公式为：转速=实际步距角/(两次有效边沿间隔的时间*60)

例：本人的步进电机步距角1.8°，四分，因此这里应该用0.45°代入公式中的步距角，有效边沿间隔时间为三次us的delay之和

若在转动时进入别的中断，可能导致的丢步和转速失调。目前本人正在研究这个问题
