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
char cmp_64BIT(STT_64BIT A,STT_64BIT B)
{
	return  (( (*( ((uint32_t *)(&(A))) +1)) > (*( ((uint32_t *)(&(B))) +1))   )  ?  1 :                                                   
		    (( (*( ((uint32_t *)(&(A))) +1)) < (*( ((uint32_t *)(&(B))) +1))   )  ? -1 :
			(( (*( ((uint32_t *)(&(A)))   )) > (*( ((uint32_t *)(&(B)))   ))   )  ?  1 : 
			(( (*( ((uint32_t *)(&(A)))   )) < (*( ((uint32_t *)(&(B)))   ))   )  ? -1 : 0 ))));
}


STT_64BIT  __div64_32(STT_64BIT *n, uint32_t base)
{
	STT_64BIT rem = *n;
	STT_64BIT b ;
	STT_64BIT res={0,0,0,0}, d = {1,0,0,0};
	uint32_t flag=0;
	uint32_t high = (*( ((uint32_t *)(n)) +1));
	(*( ((uint32_t *)(&(b)))   ))=base;
	if (high >= base) {
		high /= base;
		(*( ((uint32_t *)(&(res))) +1))=high;
		(*( ((uint32_t *)(&(rem))) +1)) -= (high*base);
	};
	while(((!((*( ((uint32_t *)(&(rem))) +1))==0 && (*( ((uint32_t *)(&(rem))) ))==0))&&
		((*( ((int32_t *)(&(rem))) +1)) >=0))
                    &&(cmp_64BIT(b,rem)<0) ){
		b=add_64BIT(b,b);
		d=add_64BIT(d,d);
	};
	do {
		if( cmp_64BIT(rem,b) >=0 ){
			rem = sub_64BIT(rem,b);
			res = add_64BIT(res,d);
		};
		flag=((*( ((uint32_t *)(&(b)))+1))&0X01);
		(*( ((uint32_t *)(&(b)))+1)) >>= 1;
		(*( ((uint32_t *)(&(b)))  )) >>= 1;
		(*( ((uint32_t *)(&(b)))  )) +=  (flag<<31) ;
		flag=((*( ((uint32_t *)(&(d)))+1))&0X01);
		(*( ((uint32_t *)(&(d)))+1)) >>= 1;
		(*( ((uint32_t *)(&(d)))  )) >>= 1;
		(*( ((uint32_t *)(&(d)))  )) +=  (flag<<31) ;
	} while ( (!((*( ((uint32_t *)(&(d))) +1))==0 && (*( ((uint32_t *)(&(d))) ))==0)) );

	*n = res;
	return rem;
}

STT_64BIT div_u64_rem(STT_64BIT dividend, uint32_t divisor, STT_64BIT *remainder)
{
	*remainder = __div64_32(&dividend, divisor);
	return dividend;
}

int main()
{
    uint64_t A=123123124;
    uint32_t B=1232;
	uint64_t savei=0;
   printf("Hello, World!%ld \n",A/B);
   printf("Hello, World!%ld \n",A%B);
	STT_64BIT C=div_u64_rem(*(STT_64BIT *)(&A),B,(STT_64BIT *)(&savei));
	printf("Hello, World!%ld \n",*(uint64_t *)(&C));
	 printf("Hello, World!%ld \n",savei);
   return 0;
}
