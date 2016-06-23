#include "ebox_common.h"

/**
 *@name     Gpio(GPIO_TypeDef *port,uint16_t pin)
 *@brief    Gpio构造函数
 *@param    port:   GPIOA,B,C,D,E,F,G
 *          pin:    GPIO_Pin_0...15
 *@retval   None
*/
Gpio::Gpio(GPIO_TypeDef *port, uint16_t pin)
{
	uint8_t temp1,temp2;
	this->port = port;
	this->pin = pin;
	switch ((uint32_t)(port))
	{
	case (uint32_t)GPIOA_BASE:
		temp1 = 0;
		break;

	case (uint32_t)GPIOB_BASE:
		temp1 = 1;
		break;

	case (uint32_t)GPIOC_BASE:
		temp1 = 2;
		break;

	case (uint32_t)GPIOD_BASE:
		temp1 = 3;
		break;

	case (uint32_t)GPIOE_BASE:
		temp1 = 4;
		break;

	case (uint32_t)GPIOF_BASE:
		temp1 = 5;
		break;

//        case (uint32_t)GPIOG_BASE:
//            temp1 = 6;
//        break;
	default:
		temp1 = 0;
		break;
	}
	for (int i = 0; i <= 15; i ++)
	{
		if ((this->pin >> i) == 0)
		{
			temp2 = i -1;
			break;
		}
	}
	this->pin_id = (PIN_ID_t)(temp1*16 + temp2);
}
/**
 *@name     void Gpio::mode(PIN_MODE mode)
 *@brief    GPIO模式设置
 *@param    mode:   PIN_MODE枚举变量类型
 *@retval   None
*/
void Gpio::mode(PIN_MODE mode,uint8_t af_configration)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	switch ((uint32_t)this->port)
	{
	case (uint32_t)GPIOA_BASE:
		//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		__HAL_RCC_GPIOA_CLK_ENABLE();
		break;

	case (uint32_t)GPIOB_BASE:
		//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		__HAL_RCC_GPIOB_CLK_ENABLE();
		break;

	case (uint32_t)GPIOC_BASE:
		//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
		__HAL_RCC_GPIOC_CLK_ENABLE();
		break;

	case (uint32_t)GPIOD_BASE:
		//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
		__HAL_RCC_GPIOD_CLK_ENABLE();
		break;

	case (uint32_t)GPIOE_BASE:
		//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
		__HAL_RCC_GPIOE_CLK_ENABLE();
		break;

	case (uint32_t)GPIOF_BASE:
		//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
		__HAL_RCC_GPIOF_CLK_ENABLE();
		break;
		// case (uint32_t)GPIOG_BASE:
		// RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
//        break;
	}


	GPIO_InitStructure.Pin = this->pin;
	GPIO_InitStructure.Mode = /*(GPIOMode_TypeDef)*/mode;
	switch ((uint8_t)mode)
	{
		/*analog input mode
		*/
	case AIN:
		//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
		GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
		//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.Pull = GPIO_NOPULL;
		break;

		/* digital input mode
		*/
	case INPUT:
		GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
		//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.Pull = GPIO_NOPULL;
		break;

	case INPUT_PD:
		GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
		//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.Pull = GPIO_PULLDOWN;
		break;

	case INPUT_PU:
		GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
		//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.Pull = GPIO_PULLUP;
		break;

		/*digital output mode
		*/
	case OUTPUT_OD:
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
		//GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
		GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStructure.Pull = GPIO_NOPULL;
		break;

	case OUTPUT_OD_PU:
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
		//GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
		GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStructure.Pull = GPIO_PULLUP;
		// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		// GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
		// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		// GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		break;

	case OUTPUT_OD_PD:
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
		//GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
		GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStructure.Pull = GPIO_PULLDOWN;
		// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		// GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
		// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		// GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
		break;

	case OUTPUT_PP:
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
		//GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
		GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStructure.Pull = GPIO_NOPULL;
		// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		// GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		// GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		break;

	case OUTPUT_PP_PU:
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
		//GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
		GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStructure.Pull = GPIO_PULLUP;
		// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		// GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		// GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		break;

	case OUTPUT_PP_PD:
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
		//GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
		GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStructure.Pull = GPIO_PULLDOWN;
		// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		// GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		// GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
		break;


		/*af mode
		*/
	case AF_OD:
		GPIO_InitStructure.Mode = GPIO_MODE_AF_OD;
		GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStructure.Pull = GPIO_NOPULL;
		GPIO_InitStructure.Alternate = af_configration;
		// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		// GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
		// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		// GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		break;

	case AF_OD_PU:
		GPIO_InitStructure.Mode = GPIO_MODE_AF_OD;
		GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStructure.Pull = GPIO_PULLUP;
		GPIO_InitStructure.Alternate = af_configration;
		// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		// GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
		// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		// GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		break;

	case AF_OD_PD:
		GPIO_InitStructure.Mode = GPIO_MODE_AF_OD;
		GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStructure.Pull = GPIO_PULLDOWN;
		GPIO_InitStructure.Alternate = af_configration;
		// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		// GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
		// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		// GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
		break;

	case AF_PP:
		GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStructure.Pull = GPIO_NOPULL;
		GPIO_InitStructure.Alternate = af_configration;
		// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		// GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		// GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		break;

	case AF_PP_PU:
		GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStructure.Pull = GPIO_PULLUP;
		GPIO_InitStructure.Alternate = af_configration;
		// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		// GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		// GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		break;

	case AF_PP_PD:
		GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStructure.Pull = GPIO_PULLDOWN;
		GPIO_InitStructure.Alternate = af_configration;
		// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		// GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		// GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
		break;
		/* if parament is other mode,set as INPUT mode
		*/
	default:
		GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
		//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.Pull = GPIO_NOPULL;
		// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//            GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		// GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		break;
	}
	// Initialization GPIO
	HAL_GPIO_Init(this->port,&GPIO_InitStructure);
	//GPIO_Init(this->port, &GPIO_InitStructure);   //初始化GPIOC端口
}
void Gpio::mode(PIN_MODE mode)
{
	Gpio::mode(mode,40);
	//af_config(af_configration);  
}

void Gpio::af_config(uint8_t af_configration)
{
	//GPIO_PinAFConfig(port, (pin_id % 16), af_configration);
}

/**
 *@name     void Gpio::set()
 *@brief    GPIO输出高电平
 *@param    NONE
 *@retval   NONE
*/
void Gpio::set()
{
	this->port->BSRR = this->pin;
}

/**
 *@name     void Gpio::reset()
 *@brief    GPIO输出低电平
 *@param    NONE
 *@retval   NONE
*/
void Gpio::reset()
{
	this->port->BSRR = this->pin << 16; // 待验证
}


/**
 *@name     void Gpio::write(uint8_t val)
 *@brief    GPIO输出设置
 *@param    val：1：输出高电平；0：输出低电平
 *@retval   NONE
*/
void Gpio::write(uint8_t val)
{
	if (val == 0)
		this->port->BSRR = this->pin << 16; // 待验证
	//this->port->BSRRH = this->pin;
	else
		//this->port->BSRRL = this->pin;
		this->port->BSRR = this->pin;
}


/**
 *@name     void Gpio::set()
 *@brief    GPIO输出高电平
 *@param    NONE
 *@retval   NONE
*/
void Gpio::read(uint8_t *val)
{
	*val = this->port->IDR & this->pin;
}


/**
 *@name     uint8_t Gpio::read(void)
 *@brief    读取GPIO引脚状态
 *@param    NONE
 *@retval   返回引脚当前的电平状态
*/
uint8_t Gpio::read(void)
{
	if (this->port->IDR & this->pin)
		return 1;
	return  0;
}

/**
 *@name     void Gpio::toggle()
 *@brief    GPIO输出电平翻转
 *@param    NONE
 *@retval   NONE
*/
void Gpio::toggle()
{
	port->ODR ^= this->pin;
}
/**
 *@name     uint8_t shift_in(Gpio *data_pin, Gpio *clock_pin, uint8_t bit_order)
 *@brief    Gpio按照时钟连续输出一个u8类型的数据，用于串行通信。
 *@param    data_pin：  数据输入引脚
            clock_pin:  时钟信号的引脚
            bit_order:  数据大小端控制LSB_FIRST：低位先发送；MSB_FIRST高位先发送
 *@retval   接收到的数据
*/
uint8_t shift_in(Gpio *data_pin, Gpio *clock_pin, uint8_t bit_order)
{
	uint8_t value = 0;
	uint8_t i;

	for (i = 0; i < 8; ++i)
	{
		clock_pin->write(HIGH);
		if (bit_order == LSB_FIRST)
			value |= data_pin->read() << i;
		else
			value |= data_pin->read() << (7 - i);
		clock_pin->write(LOW);
	}
	return value;
}

/**
 *@name     uint8_t shift_in(Gpio *data_pin, Gpio *clock_pin, uint8_t bit_order)
 *@brief    Gpio按照时钟连续输出一个u8类型的数据，用于串行通信。
 *@param    data_pin：  数据输出引脚
            clock_pin:  时钟信号的引脚
            bit_order:  数据大小端控制LSB_FIRST：低位先发送；MSB_FIRST高位先发送
            val：       要发送的数据
 *@retval   NONE
*/
void shift_out(Gpio *data_pin, Gpio *clock_pin, uint8_t bit_order, uint8_t val)
{
	int i;
	for (i = 0; i < 8; i++)
	{
		if (bit_order == LSB_FIRST)
			data_pin->write(!!(val & (1 << i)));
		else
			data_pin->write(!!(val & (1 << (7 - i))));

		clock_pin->write(HIGH);
		clock_pin->write(LOW);

	}
}
