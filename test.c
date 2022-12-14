//    请在支持64位的机器上测试
//Please test on a machine that supports 64-bit

// 已经在 https://www.runoob.com/try/runcode.php?filename=helloworld&type=c  上测试过
// Has been tested on https://www.runoob.com/try/runcode.php?filename=helloworld&type=c

#include <stdio.h>
#include <limits.h>

#if (CHAR_BIT==8)
	typedef  char                   int8_t  ;
	typedef  unsigned char          uint8_t ;
#else
	#error sizeof(char)不等于1字节
#endif
#if (USHRT_MAX==65535)
	typedef  short                  int16_t ;
	typedef  unsigned short         uint16_t;
#else
	#error sizeof(short)不等于2字节
#endif

#if (UINT_MAX==4294967295)
	typedef  int                    int32_t ;
	typedef  unsigned int           uint32_t;
#elif (ULONG_MAX==4294967295)
	typedef  long                   int32_t ;
	typedef  unsigned long          uint32_t;
#else 
	#error sizeof(int)或sizeof(long) 不等于4字节
#endif
#if (LONG_MAX==9223372036854775807)
	typedef  long                   int64_t ;
	typedef  unsigned long          uint64_t;
#else 
	//#error 不支持 64位
#endif
struct STT_TAG_64BIT
{
    uint16_t L_L;
    uint16_t L_H;
    uint16_t H_L;
    uint16_t H_H;
} ;
typedef struct STT_TAG_64BIT STT_64BIT;

extern char      cmp_64BIT(const STT_64BIT  A,const STT_64BIT  B);
extern STT_64BIT sub_64BIT(const STT_64BIT  A,const STT_64BIT  B);
extern STT_64BIT add_64BIT(const STT_64BIT  A,const STT_64BIT  B);
extern STT_64BIT mul_64BIT(const STT_64BIT  A,const STT_64BIT  B);
extern STT_64BIT div_64BIT(STT_64BIT dividend, uint32_t divisor, STT_64BIT *remainder);

//比较
char cmp_64BIT(const STT_64BIT A,const STT_64BIT B)//149
{
	register uint32_t  A_D;
	register uint32_t  B_D;	
	register uint32_t * A_P=(uint32_t *)(&A);
	register uint32_t * B_P=(uint32_t *)(&B);
	 {
	    A_D=(*( A_P +1));
	    B_D=(*( B_P +1));	
	    if(A_D==B_D){goto GOTO_BREAK;};
	   goto GOTO_END;
	 };
	 GOTO_BREAK: ;
	A_D=(*( A_P ));
	B_D=(*( B_P ));	
	if(A_D==B_D){return 0;};
	GOTO_END: ;
	return (A_D>B_D ?1:-1);
}
//减法
STT_64BIT sub_64BIT(const STT_64BIT  A,const STT_64BIT  B)
{
	STT_64BIT ret={0,0,0,0};

	(*( ((uint32_t *)(&(ret))) +1)) =
	(*( ((uint32_t *)(&(A))) +1))  - (*( ((uint32_t *)(&(B))) +1));    // 高位相减
	if ( 
		(  (*( ((uint32_t *)(&(ret))))) =(*( ((uint32_t *)(&(A))))) - (*( ((uint32_t *)(&(B)))))  )
		> (*( ((uint32_t *)(&(A))))) 
	 ) {                // 若低位不够减，则向高位借位
		(*( ((uint32_t *)(&(ret))) +1))--;
	};
	return ret;
}
//加法
STT_64BIT add_64BIT(const STT_64BIT  A,const STT_64BIT  B)
{
	STT_64BIT ret={0,0,0,0};

	(*( ((uint32_t *)(&(ret))) +1)) =
	(*( ((uint32_t *)(&(A))) +1))  + (*( ((uint32_t *)(&(B))) +1));    // 高位相加
	if ( 
		(  (*( ((uint32_t *)(&(ret))))) =(*( ((uint32_t *)(&(A))))) + (*( ((uint32_t *)(&(B)))))  )
		< (*( ((uint32_t *)(&(A))))) 
	 ) {                // 若低位相加大于0xFFFFFFFF  则向高位加1
		(*( ((uint32_t *)(&(ret))) +1))++;
	};
	return ret;
}


/* 
 * unsigned 64位除法，需要的得到余数
 * Param - u64	: 被除数
 * Param - u32	: 除数
 * Param - u64* : 除后的余数
 * Return - u64	: 除后的结果
 */
STT_64BIT  div_64BIT(STT_64BIT rem, uint32_t base, STT_64BIT *remainder)
{
	STT_64BIT  b={0,0,0,0}, res={0,0,0,0}, d = {1,0,0,0};
	uint32_t *  res_P =(uint32_t *)(&res);
	uint32_t *	rem_P =(uint32_t *)(&rem);
	uint32_t *	b_P   =(uint32_t *)(&b);
	uint32_t *	d_P   =(uint32_t *)(&d);
	{
		uint32_t    high  =(*( ((uint32_t *)(&rem)) +1));
		b_P[0]=base;
		if (high >= base) {
			high /= base;
			res_P[1]  =  high;
			rem_P[1]  -= (high*base);
		};
	};
	while(  ( rem_P[1] !=0 || rem_P[0]!=0 )
		     &&( ((int32_t *)rem_P)[1] >=0) //&&(cmp_64BIT(b,rem)<0)
	         &&(( b_P[1]== rem_P[1] ) ?
			   (( b_P[0]== rem_P[0] ) ? 0 : 
			   (( b_P[0] > rem_P[0] ) ? 0 : 1))
		                                  :
		       (( b_P[1] > rem_P[1] ) ? 0 : 1)) ){
		b_P[1] <<= 1;
		if(( ((uint8_t *)b_P)[3] & (0X01<<7) )!=0){
			((uint8_t *)b_P)[4]++;
		};
		b_P[0] <<= 1;
		d_P[1] <<= 1;
		if(( ((uint8_t *)d_P)[3] & (0X01<<7) )!=0){
			((uint8_t *)d_P)[4]++;
		};
		d_P[0] <<= 1;
	};
	do {
		if(    (( rem_P[1]== b_P[1] ) ?
			   (( rem_P[0]== b_P[0] ) ? 1 : 
			   (( rem_P[0] > b_P[0] ) ? 1 : 0))
		                                  :
		       (( rem_P[1] > b_P[1] ) ? 1 : 0)) ){//cmp_64BIT(rem,b) >=0
	rem_P[1] -= (b_P[1] + (( rem_P[0] <  b_P[0] ) ? 1 : 0)) ; 
	rem_P[0] -=  b_P[0];//rem = sub_64BIT(rem,b);
	res_P[0] +=  d_P[0];//res = add_64BIT(res,d);
	res_P[1] += (d_P[1] + (( res_P[0] <  d_P[0] ) ? 1 : 0)) ; 	
		};
		b_P[0] >>= 1;
		if(( ((uint8_t *)b_P)[4] & (0X01) )!=0){
			((uint8_t *)b_P)[3] |= (0X01<<7);
		};
		b_P[1] >>= 1;
		d_P[0] >>= 1;
		if(( ((uint8_t *)d_P)[4] & (0X01) )!=0){
			((uint8_t *)d_P)[3] |= (0X01<<7);
		};
		d_P[1] >>= 1;
	} while ( d_P[1]!=0 || d_P[0]!=0 );
	if(remainder!=NULL){
		*remainder = rem;
	}
	return res;
}
//乘法
STT_64BIT mul_64BIT(const STT_64BIT  A,const STT_64BIT  B)
{
	char j,i;
	STT_64BIT   ret={0,0,0,0};
	uint16_t    tempdata[5]={0,0,0,0,0};
	STT_64BIT * temp=(STT_64BIT *)tempdata;
	for(j=0;j<=3 ;j++){
		for(i=0;i<=3 ;i++ ){
			if((i+j)>3){continue;};
			((uint32_t *)(temp))[0]=0;
			((uint32_t *)(temp))[1]=0;
			* ((uint32_t *)(((uint16_t *)(temp)) +i+j))=
			(((uint32_t)(* (((uint16_t*)(&A))+j) )) * ((uint32_t)(* (((uint16_t*)(&B))+i) )));
			ret=add_64BIT(*temp,ret);
		};
	};
	return ret;
}
int main()//测除法和余数法
{
{
    uint64_t A=123123124;
    uint32_t B=1232;
	uint64_t savei=0;
   printf("Hello, World!%ld \n",A/B);
   printf("Hello, World!%ld \n",A%B);
	STT_64BIT C=div_64BIT(*(STT_64BIT *)(&A),B,(STT_64BIT *)(&savei));
	printf("Hello, World!%ld \n",*(uint64_t *)(&C));
	 printf("Hello, World!%ld \n",savei);
//   return 0;
}
//int main()//测比较大小
{
    uint64_t A=1232123213;
    uint64_t B=1232123213;
    char C=cmp_64BIT(*(STT_64BIT *)(&A),*(STT_64BIT *)(&B));
    printf("Hello, World!%hhd \n",C);
//   return 0;
}
//int main()//测加法
{
    uint64_t A=1232123213;
    uint64_t B=1232123213;
    STT_64BIT C=add_64BIT(*(STT_64BIT *)(&A),*(STT_64BIT *)(&B));
    printf("Hello, World!%llu \n",A+B);
    printf("Hello, World!%llu \n",*(uint64_t *)(&C));
//   return 0;
}
//int main()//测减法
{
    uint64_t A=1232123213;
    uint64_t B=1232123213;
    STT_64BIT C=sub_64BIT(*(STT_64BIT *)(&A),*(STT_64BIT *)(&B));
    printf("Hello, World!%llu \n",A-B);
    printf("Hello, World!%llu \n",*(uint64_t *)(&C));
//   return 0;
}
//int main()//测乘法
{
    uint64_t A=1232123213;
    uint64_t B=1232123213;
    STT_64BIT C=mul_64BIT(*(STT_64BIT *)(&A),*(STT_64BIT *)(&B));
    printf("Hello, World!%llu \n",A*B);
    printf("Hello, World!%llu \n",*(uint64_t *)(&C));
 //  return 0;
}
}
