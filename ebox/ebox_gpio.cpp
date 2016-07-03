#include "ebox_common.h"


/**
 *@name     Gpio(GPIO_TypeDef *port,uint16_t pin)
 *@brief    Gpio���캯��
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
 *@brief    GPIOģʽ����
 *@param    mode:   PIN_MODEö�ٱ�������
 *@retval   None
*/
void Gpio::mode(PIN_MODE mode,uint8_t af_configration)
{
	//GPIO_InitTypeDef GPIO_InitStructure;

	switch ((uint32_t)this->port)
	{
	case (uint32_t)GPIOA_BASE:
		//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		__HAL_RCC_GPIOA_CLK_ENABLE();
		//LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA)
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
	}


//	GPIO_InitStructure.Pin = this->pin;
	// GPIO_InitStructure.Pin = this->pin;
//	GPIO_InitStructure.Mode = /*(GPIOMode_TypeDef)*/mode;
	switch ((uint8_t)mode)
	{
		/*analog input mode
		*/
	case AIN:
		// GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
		// GPIO_InitStructure.Pull = GPIO_NOPULL;
		LL_GPIO_SetPinMode(port,pin,LL_GPIO_MODE_ANALOG);
		LL_GPIO_SetPinPull(port,pin,LL_GPIO_PULL_NO);
		
		break;

		/* digital input mode
		*/
	case INPUT:
		// GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
		// GPIO_InitStructure.Pull = GPIO_NOPULL;
		
		LL_GPIO_SetPinMode(port,pin,LL_GPIO_MODE_INPUT);
		LL_GPIO_SetPinPull(port,pin,LL_GPIO_PULL_NO);
		break;

	case INPUT_PD:
		// GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
		// GPIO_InitStructure.Pull = GPIO_PULLDOWN;
		
				LL_GPIO_SetPinMode(port,pin,LL_GPIO_MODE_INPUT);
		LL_GPIO_SetPinPull(port,pin,LL_GPIO_PULL_DOWN);
		break;

	case INPUT_PU:
		// GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
		// GPIO_InitStructure.Pull = GPIO_PULLUP;
				LL_GPIO_SetPinMode(port,pin,LL_GPIO_MODE_INPUT);
		LL_GPIO_SetPinPull(port,pin,LL_GPIO_PULL_UP);
		break;

		/*digital output mode
		*/
	case OUTPUT_OD:
		// GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
		// GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
		// GPIO_InitStructure.Pull = GPIO_NOPULL;
		LL_GPIO_SetPinMode(port,pin,LL_GPIO_MODE_OUTPUT);
		LL_GPIO_SetPinOutputType(port,pin,LL_GPIO_OUTPUT_OPENDRAIN);
		LL_GPIO_SetPinSpeed(port,pin,LL_GPIO_SPEED_FREQ_HIGH);		
		LL_GPIO_SetPinPull(port,pin,LL_GPIO_PULL_NO);
		break;

	case OUTPUT_OD_PU:
		// GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
		// GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
		// GPIO_InitStructure.Pull = GPIO_PULLUP;
		LL_GPIO_SetPinMode(port,pin,LL_GPIO_MODE_OUTPUT);
		LL_GPIO_SetPinOutputType(port,pin,LL_GPIO_OUTPUT_OPENDRAIN);
		LL_GPIO_SetPinSpeed(port,pin,LL_GPIO_SPEED_FREQ_HIGH);		
		LL_GPIO_SetPinPull(port,pin,LL_GPIO_PULL_UP);;
		break;

	case OUTPUT_OD_PD:
		// GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
		// GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
		// GPIO_InitStructure.Pull = GPIO_PULLDOWN;
		LL_GPIO_SetPinMode(port,pin,LL_GPIO_MODE_OUTPUT);
		LL_GPIO_SetPinOutputType(port,pin,LL_GPIO_OUTPUT_OPENDRAIN);
		LL_GPIO_SetPinSpeed(port,pin,LL_GPIO_SPEED_FREQ_HIGH);		
		LL_GPIO_SetPinPull(port,pin,LL_GPIO_PULL_DOWN);
		break;

	case OUTPUT_PP:
		// GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
		// GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
		// GPIO_InitStructure.Pull = GPIO_NOPULL;
		LL_GPIO_SetPinMode(port,pin,LL_GPIO_MODE_OUTPUT);
		LL_GPIO_SetPinOutputType(port,pin,LL_GPIO_OUTPUT_PUSHPULL);
		LL_GPIO_SetPinSpeed(port,pin,LL_GPIO_SPEED_FREQ_HIGH);		
		LL_GPIO_SetPinPull(port,pin,LL_GPIO_PULL_NO);
		break;

	case OUTPUT_PP_PU:
		// GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
		// GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
		// GPIO_InitStructure.Pull = GPIO_PULLUP;
		LL_GPIO_SetPinMode(port,pin,LL_GPIO_MODE_OUTPUT);
		LL_GPIO_SetPinOutputType(port,pin,LL_GPIO_OUTPUT_PUSHPULL);
		LL_GPIO_SetPinSpeed(port,pin,LL_GPIO_SPEED_FREQ_HIGH);		
		LL_GPIO_SetPinPull(port,pin,LL_GPIO_PULL_UP);
		break;

	case OUTPUT_PP_PD:
		// GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
		// GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
		// GPIO_InitStructure.Pull = GPIO_PULLDOWN;
		LL_GPIO_SetPinMode(port,pin,LL_GPIO_MODE_OUTPUT);
		LL_GPIO_SetPinOutputType(port,pin,LL_GPIO_OUTPUT_PUSHPULL);
		LL_GPIO_SetPinSpeed(port,pin,LL_GPIO_SPEED_FREQ_HIGH);		
		LL_GPIO_SetPinPull(port,pin,LL_GPIO_PULL_DOWN);
		

		break;


		/*af mode
		*/
	case AF_OD:
		// GPIO_InitStructure.Mode = GPIO_MODE_AF_OD;
		// GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
		// GPIO_InitStructure.Pull = GPIO_NOPULL;
		// GPIO_InitStructure.Alternate = af_configration;
		LL_GPIO_SetPinMode(port,pin,LL_GPIO_MODE_ALTERNATE);
		LL_GPIO_SetPinOutputType(port,pin,LL_GPIO_OUTPUT_OPENDRAIN);
		LL_GPIO_SetPinSpeed(port,pin,LL_GPIO_SPEED_FREQ_HIGH);		
		LL_GPIO_SetPinPull(port,pin,LL_GPIO_PULL_NO);
		if (pin< LL_GPIO_PIN_8)
		{
			LL_GPIO_SetAFPin_0_7(port,pin,af_configration);
		}else{
			LL_GPIO_SetAFPin_8_15(port,pin,af_configration);
		}
		break;

	case AF_OD_PU:
		// GPIO_InitStructure.Mode = GPIO_MODE_AF_OD;
		// GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
		// GPIO_InitStructure.Pull = GPIO_PULLUP;
		// GPIO_InitStructure.Alternate = af_configration;
		LL_GPIO_SetPinMode(port,pin,LL_GPIO_MODE_ALTERNATE);
		LL_GPIO_SetPinOutputType(port,pin,LL_GPIO_OUTPUT_OPENDRAIN);
		LL_GPIO_SetPinSpeed(port,pin,LL_GPIO_SPEED_FREQ_HIGH);		
		LL_GPIO_SetPinPull(port,pin,LL_GPIO_PULL_UP);
		if (pin< LL_GPIO_PIN_8)
		{
			LL_GPIO_SetAFPin_0_7(port,pin,af_configration);
		}else{
			LL_GPIO_SetAFPin_8_15(port,pin,af_configration);
		}
		break;

	case AF_OD_PD:
		// GPIO_InitStructure.Mode = GPIO_MODE_AF_OD;
		// GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
		// GPIO_InitStructure.Pull = GPIO_PULLDOWN;
		// GPIO_InitStructure.Alternate = af_configration;
		LL_GPIO_SetPinMode(port,pin,LL_GPIO_MODE_ALTERNATE);
		LL_GPIO_SetPinOutputType(port,pin,LL_GPIO_OUTPUT_OPENDRAIN);
		LL_GPIO_SetPinSpeed(port,pin,LL_GPIO_SPEED_FREQ_HIGH);		
		LL_GPIO_SetPinPull(port,pin,LL_GPIO_PULL_DOWN);
		if (pin< LL_GPIO_PIN_8)
		{
			LL_GPIO_SetAFPin_0_7(port,pin,af_configration);
		}else{
			LL_GPIO_SetAFPin_8_15(port,pin,af_configration);
		}
		break;

	case AF_PP:
		// GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
		// GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
		// GPIO_InitStructure.Pull = GPIO_NOPULL;
		// GPIO_InitStructure.Alternate = af_configration;
		LL_GPIO_SetPinMode(port,pin,LL_GPIO_MODE_ALTERNATE);
		LL_GPIO_SetPinOutputType(port,pin,LL_GPIO_OUTPUT_PUSHPULL);
		LL_GPIO_SetPinSpeed(port,pin,LL_GPIO_SPEED_FREQ_HIGH);		
		LL_GPIO_SetPinPull(port,pin,LL_GPIO_PULL_NO);
		if (pin< LL_GPIO_PIN_8)
		{
			LL_GPIO_SetAFPin_0_7(port,pin,af_configration);
		}else{
			LL_GPIO_SetAFPin_8_15(port,pin,af_configration);
		}

		break;

	case AF_PP_PU:
		// GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
		// GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
		// GPIO_InitStructure.Pull = GPIO_PULLUP;
		// GPIO_InitStructure.Alternate = af_configration;
		
		LL_GPIO_SetPinMode(port,pin,LL_GPIO_MODE_ALTERNATE);
		LL_GPIO_SetPinOutputType(port,pin,LL_GPIO_OUTPUT_PUSHPULL);
		LL_GPIO_SetPinSpeed(port,pin,LL_GPIO_SPEED_FREQ_HIGH);		
		LL_GPIO_SetPinPull(port,pin,LL_GPIO_PULL_UP);
		if (pin< LL_GPIO_PIN_8)
		{
			LL_GPIO_SetAFPin_0_7(port,pin,af_configration);
		}else{
			LL_GPIO_SetAFPin_8_15(port,pin,af_configration);
		}
		break;

	case AF_PP_PD:
		// GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
		// GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
		// GPIO_InitStructure.Pull = GPIO_PULLDOWN;
		// GPIO_InitStructure.Alternate = af_configration;
		LL_GPIO_SetPinMode(port,pin,LL_GPIO_MODE_ALTERNATE);
		LL_GPIO_SetPinOutputType(port,pin,LL_GPIO_OUTPUT_PUSHPULL);
		LL_GPIO_SetPinSpeed(port,pin,LL_GPIO_SPEED_FREQ_HIGH);		
		LL_GPIO_SetPinPull(port,pin,LL_GPIO_PULL_DOWN);
		if (pin< LL_GPIO_PIN_8)
		{
			LL_GPIO_SetAFPin_0_7(port,pin,af_configration);
		}else{
			LL_GPIO_SetAFPin_8_15(port,pin,af_configration);
		}
		

		break;
		/* if parament is other mode,set as INPUT mode
		*/
	default:
		//GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	    //GPIO_InitStructure.Pull = GPIO_NOPULL;
		LL_GPIO_SetPinMode(port,pin,LL_GPIO_MODE_INPUT);
		LL_GPIO_SetPinPull(port,pin,LL_GPIO_PULL_NO);
	
		break;
	}
	//GPIO_Init(this->port, &GPIO_InitStructure);   //��ʼ��GPIOC�˿�
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
 *@brief    GPIO����ߵ�ƽ
 *@param    NONE
 *@retval   NONE
*/
void Gpio::set()
{
	this->port->BSRR = this->pin;
}

/**
 *@name     void Gpio::reset()
 *@brief    GPIO����͵�ƽ
 *@param    NONE
 *@retval   NONE
*/
void Gpio::reset()
{
	this->port->BSRR = this->pin << 16; // ����֤
}


/**
 *@name     void Gpio::write(uint8_t val)
 *@brief    GPIO�������
 *@param    val��1������ߵ�ƽ��0������͵�ƽ
 *@retval   NONE
*/
void Gpio::write(uint8_t val)
{
	if (val == 0)
		this->port->BSRR = this->pin << 16; // ����֤
	//this->port->BSRRH = this->pin;
	else
		//this->port->BSRRL = this->pin;
		this->port->BSRR = this->pin;
}


/**
 *@name     void Gpio::set()
 *@brief    GPIO����ߵ�ƽ
 *@param    NONE
 *@retval   NONE
*/
void Gpio::read(uint8_t *val)
{
	*val = this->port->IDR & this->pin;
}


/**
 *@name     uint8_t Gpio::read(void)
 *@brief    ��ȡGPIO����״̬
 *@param    NONE
 *@retval   �������ŵ�ǰ�ĵ�ƽ״̬
*/
uint8_t Gpio::read(void)
{
	if (this->port->IDR & this->pin)
		return 1;
	return  0;
}

/**
 *@name     void Gpio::toggle()
 *@brief    GPIO�����ƽ��ת
 *@param    NONE
 *@retval   NONE
*/
void Gpio::toggle()
{
	port->ODR ^= this->pin;
}
/**
 *@name     uint8_t shift_in(Gpio *data_pin, Gpio *clock_pin, uint8_t bit_order)
 *@brief    Gpio����ʱ���������һ��u8���͵����ݣ����ڴ���ͨ�š�
 *@param    data_pin��  ������������
            clock_pin:  ʱ���źŵ�����
            bit_order:  ���ݴ�С�˿���LSB_FIRST����λ�ȷ��ͣ�MSB_FIRST��λ�ȷ���
 *@retval   ���յ�������
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
 *@brief    Gpio����ʱ���������һ��u8���͵����ݣ����ڴ���ͨ�š�
 *@param    data_pin��  �����������
            clock_pin:  ʱ���źŵ�����
            bit_order:  ���ݴ�С�˿���LSB_FIRST����λ�ȷ��ͣ�MSB_FIRST��λ�ȷ���
            val��       Ҫ���͵�����
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
