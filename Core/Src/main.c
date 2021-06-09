/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * FILE           : main.c
  * PROJECT        : vasudevan_assignment4
  * PROGRAMMER     : Pranav Manakkulamparambil Vasudevan
  * FIRST VERSION  : 2020-04-12
  * DESCRIPTION    : Code for implementing a simplified Debit card reader machine
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stdio.h"
#include "debounce.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
char stringBuffer[16]= {0}; // string value for sprintf function for the lcd


/*FUNCTION   :void printWelcome()
 * DESCRIPTION: function for printing welcome on lcd
 * PARAMETERS : Nothing
 * RETURNS    : Nothing
 */

void printWelcome()
{
	HD44780_Init(); // initializing lcd
	HD44780_GotoXY(4, 0);
	HD44780_PutStr("Welcome");
//	HAL_Delay(1);
}

/*FUNCTION   :void printAmount()
 * DESCRIPTION: function for printing random amount on lcd
 * PARAMETERS : Nothing
 * RETURNS    : Nothing
 */

void printAmount()
{
	HD44780_ClrScr(); //clear lcd
	HD44780_GotoXY(4, 0);
	HD44780_PutStr("$12.34");
	HD44780_GotoXY(0, 1);
	HD44780_PutStr("Press Ok or Can");

}
/*FUNCTION   :void printAccount()
 * DESCRIPTION: function for printing account type on lcd
 * PARAMETERS : Nothing
 * RETURNS    : Nothing
 */

void printAccount()
{
	HD44780_ClrScr(); //clear lcd
	HD44780_PutStr("Press 1 for Chequing");
	HD44780_GotoXY(0, 1);
	HD44780_PutStr("and 2 for Savings");
}
/*FUNCTION   :void printCancelled()
 * DESCRIPTION: function for printing "transaction cancelled" on lcd
 * PARAMETERS : Nothing
 * RETURNS    : Nothing
 */

void printCancelled()
{
	HD44780_ClrScr(); //clear lcd
	HD44780_PutStr("Transaction");
	HD44780_GotoXY(0, 1);
	HD44780_PutStr("Cancelled");
	HAL_Delay(10000);
}
/*FUNCTION   :void printEnterpin()
 * DESCRIPTION: function for printing "Enter pin" on lcd
 * PARAMETERS : Nothing
 * RETURNS    : Nothing
 */

void printEnterpin()
{
	HD44780_ClrScr(); //clear lcd
	HD44780_PutStr("Enter pin");
	HAL_Delay(1);
}
/*FUNCTION   :void printProcessing()
 * DESCRIPTION: function for printing "Processing" on lcd for 10seconds
 * PARAMETERS : Nothing
 * RETURNS    : Nothing
 */
void printProcessing()
{
	//print processing
	HD44780_ClrScr(); //clear lcd
	HD44780_PutStr("Processing...");
	HAL_Delay(10000);
	printf("processing\r\n");
}
/*FUNCTION   :void printIncorrect()
 * DESCRIPTION: function for printing "Incorrect pin"  on lcd
 * PARAMETERS : Nothing
 * RETURNS    : Nothing
 */
void printIncorrect()
{
	//printing incorrect pin
	HD44780_ClrScr(); //clear lcd
	HD44780_PutStr("Incorrect pin");
	HAL_Delay(2000);
}
/*FUNCTION   :void printApproved()
 * DESCRIPTION: function for printing transaction appoval  on lcd
 * PARAMETERS : Nothing
 * RETURNS    : Nothing
 */
void printApproved()
{
	//printing transaction approved
	HD44780_ClrScr(); //clear lcd
	HD44780_PutStr("Approved");
	HD44780_GotoXY(0, 1); //set cursor to the second row of the lcd
	HD44780_PutStr("Thank you");
	HAL_Delay(2000);
}
/*FUNCTION   :void printNobalance()
 * DESCRIPTION: function for printing insufficient balance statement on lcd
 * PARAMETERS : Nothing
 * RETURNS    : Nothing
 */
void printNobalance()
{
	HD44780_ClrScr(); //clear lcd
	HD44780_PutStr("Insufficient");
	HD44780_GotoXY(0, 1); //set cursor to the second row of the lcd
	HD44780_PutStr("Balance");
	HAL_Delay(2000);
}

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	//setting up GPIO pins
	uint16_t pin= GPIO_PIN_3;//button for 1
	uint16_t pin2= GPIO_PIN_4;//button for 2
	uint16_t pin3= GPIO_PIN_7;//button for 3
	uint16_t pin4= GPIO_PIN_8;//button for 4

	char port= 'A';//setting port
	int8_t mode = 0;//setting mode
	//initializing ports
	int8_t pA3=1;
	int8_t pA4=1;
	int8_t pA7=1;
	int8_t pA8=1;

	int16_t passCode[]={1111,1121,1133,1213,2111,2331,3111,3211,3333};//preassigning passcodes
	int16_t passWord[3]={0};//password entered by user
	int16_t passwordInt=0;// variable to store the interger value of password entered by user
//initializing the debounce
	deBounceInit( pin, port, mode);
	deBounceInit2( pin2, port, mode);
	deBounceInit3( pin3, port, mode);
	deBounceInit4( pin4, port, mode);

	int16_t num=0;//for differentiate which button is pressed
	int16_t buttonPressed=0;
	int16_t tranState=0;// state machine state
	int16_t i=0;//counter1
	int16_t j=0;//counter2
	int16_t flag=0;

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */
 
 

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
     switch(tranState)//switch statement for state machine
     {
     case 0: printWelcome();//function call to print welcome
         	int16_t passwordInt=0;//initializing password integer
        	printf("Welcome\r\n");
        	//getting the input from ok button
    	   	pA8 = deBounceReadPin4(pin4, port, mode);
            if(pA8==0)
            {
            	tranState=1;
            	break;
            }
            else
            {
            	tranState=0;
            	break;
            }
     case 1:
    	    printAmount();//function call to print amount
    	    printf("12.4\r\n press ok or canc\r\n");
    	    // getting input from button 1 and 2
    	  	pA3 = deBounceReadPin(pin, port, mode);
    		pA4 = deBounceReadPin2(pin2, port, mode);
    		if(pA3==0)
    		{
    			tranState=2;
    			break;
    		}
    		else if(pA4==0)
    		{
    			tranState=13;
    			break;

    		}
    		else
    		{
    			tranState=1;
    			break;
    		}

		case 2:
			printAccount();//fuction call to print account type
			printf("Press 1 for Chequing\r\nand 2 for Savings\r\n");
    	    // getting input from button 1 and 2
    	  	pA3 = deBounceReadPin(pin, port, mode);
    		pA4 = deBounceReadPin2(pin2, port, mode);
    		if(pA3==0)
    		{
    			tranState=3;
    			break;
    		}
    		else if(pA4==0)
			{
				tranState=3;
				break;
			}
			else
			{
				tranState=2;
				break;
			}

		case 3:
			printEnterpin();//function to print enter pin
			printf("Enter pin\r\n");
			//getting input from all the buttons for first number of password
			pA3 = deBounceReadPin(pin, port, mode);
			pA4 = deBounceReadPin2(pin2, port, mode);
			pA7 = deBounceReadPin3(pin3, port, mode);
		    pA8 = deBounceReadPin4(pin4, port, mode);
		    if(pA3==0)
		    {
		    	passWord[0]=1;
		    	tranState=4;
		    	break;
		    }
		    else if(pA4==0)
		    {
		    	passWord[0]=2;
		    	tranState=4;
		    	break;
		    }
		    else if (pA7==0)
		    {
		    	passWord[0]=3;
		    	tranState=4;
		    	break;
		    }
		    else
		    {
		    	tranState=3;
		    	break;
		    }

		case 4:
			    HD44780_GotoXY(0, 1); //set cursor to the second row of the lcd
				HD44780_PutStr("*");//printing *
				HAL_Delay(1);
				printf("*");
				//getting input from all the buttons for second number of password
				pA3 = deBounceReadPin(pin, port, mode);
		        pA4 = deBounceReadPin2(pin2, port, mode);
		        pA7 = deBounceReadPin3(pin3, port, mode);
		        pA8 = deBounceReadPin4(pin4, port, mode);
		        if(pA3==0)
		        {
	    	     passWord[1]=1;
	    	     tranState=5;
	    	     break;
	            }
	          else if(pA4==0)
	           {
	     	     passWord[1]=2;
	    	     tranState=5;
	    	     break;
	            }
	         else if (pA7==0)
	          {
	        	passWord[1]=3;
	        	tranState=5;
	    	    break;
	          }
	         else
	         {
	        	tranState=4;
	        	break;
	         }
		case 5:
			HD44780_GotoXY(0, 1); //set cursor to the second row of the lcd
			HD44780_PutStr("**");
			printf("*");
			HAL_Delay(1);
			//getting input from all the buttons for third number of password
			pA3 = deBounceReadPin(pin, port, mode);
	        pA4 = deBounceReadPin2(pin2, port, mode);
	        pA7 = deBounceReadPin3(pin3, port, mode);
	        pA8 = deBounceReadPin4(pin4, port, mode);
	        if(pA3==0)
	        {
    	     passWord[2]=1;
    	     tranState=6;
    	     break;
            }
          else if(pA4==0)
           {
     	     passWord[2]=2;
    	     tranState=6;
    	     break;
            }
         else if (pA7==0)
          {
        	passWord[2]=3;
        	tranState=6;
    	    break;
          }
         else
         {
        	tranState=5;
        	break;
         }
		case 6:
			HD44780_GotoXY(0, 1); //set cursor to the second row of the lcd
			HD44780_PutStr("***");
			printf("*");
			HAL_Delay(1);
			//getting input from all the buttons for fourth number of password
			pA3 = deBounceReadPin(pin, port, mode);
	        pA4 = deBounceReadPin2(pin2, port, mode);
	        pA7 = deBounceReadPin3(pin3, port, mode);
	        pA8 = deBounceReadPin4(pin4, port, mode);
	        if(pA3==0)
	        {
    	     passWord[3]=1;
    	     tranState=7;
    	     break;
            }
          else if(pA4==0)
           {
     	     passWord[3]=2;
    	     tranState=7;
    	     break;
            }
         else if (pA7==0)
          {
        	passWord[3]=3;
        	tranState=7;
    	    break;
          }
         else
         {
        	tranState=6;
        	break;
         }
			

		case 7:
			HD44780_GotoXY(0, 1); //set cursor to the second row of the lcd
			HD44780_PutStr("****");
			printf("*");
			HAL_Delay(1);
			//getting input from ok button
			pA8 = deBounceReadPin4(pin4, port, mode);
		    if(pA8==0)
			{
			 tranState=8;
			 break;
		    }
			else
			{
			tranState=7;
			break;
		    }
		case 8:
			//print processing
			printProcessing();
			//converting ented password into interger value
			for(i=0;i<4;i++)
			{
				passwordInt= passwordInt*10;
				passwordInt= passwordInt+passWord[i];
			}
			printf("Entered password=%d\r\n",passwordInt);
			tranState=9;
			break;

		case 9:
			//checking for the passcode match
            for(j=0;j<9;j++)
            {
            	if(passCode[j]==passwordInt)
            	{
            		flag=1;
            	}
            }
            if(flag==1)
            {
            	tranState=11;
            	break;
            }
            else
            {
            	tranState=10;
            	break;
            }

		case 10:
			//printing incorrect pin
			printIncorrect();
			printf("Incorrect pin\r\n");
			tranState=13;
			break;

		case 11:
			//checking for minimum balance
			if(passwordInt==3333)
			{
				tranState=12;
				break;
			}
			else
			{   //printing transaction approved
				printApproved();
				printf("Approved\r\n Thank you\r\n");
				tranState=0;
				break;
			}
		case 12:
			printNobalance();
			printf("Insufficinet \r\n Balance\r\n");
			tranState=13;
			break;

		case 13:
			 printCancelled();//function call to print cancel statement
			 printf("Transaction\r\n Cancelled\r\n");
			 tranState=0;
 	         break;
     }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure LSE Drive Capability 
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the main internal regulator output voltage 
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable MSI Auto calibration 
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|LD3_Pin|GPIO_PIN_4 
                          |GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA3 PA4 PA7 PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_7|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 LD3_Pin PB4 
                           PB5 PB6 PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|LD3_Pin|GPIO_PIN_4 
                          |GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
