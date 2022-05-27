#include "show.h"
#include "oled.h"

extern float length;
//extern：置于变量或者函数前，以表示变量或者函数的定义在别的文件中，提示编译器遇到此变量和函数时在其他模块中寻找其定义。
//这里面要注意，对于 extern 申明变量可以多次，但定义只有一次。

void oled_show(void)
{
	int int_length,decimal_lenght;
	int_length = (uint16_t)length;
	decimal_lenght = length * 1000 - int_length * 1000;	
	
	OLED_ShowString(00,00,"HCSR:");
	OLED_ShowNum(37,00,int_length,4,12);
	OLED_ShowString(63,00,".");
	OLED_ShowNum(69,00,decimal_lenght,3,12);
	OLED_ShowString(88,00,"cm");	
}
