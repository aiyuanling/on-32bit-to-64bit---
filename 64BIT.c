/*
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
*/
////////////////////////////////////////////////////////////////////////////////
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
char cmp_64BIT(const STT_64BIT A,const STT_64BIT B)
{
	register uint32_t * A_P=(uint32_t *)(&A);
	register uint32_t * B_P=(uint32_t *)(&B);	
        if( A_P[1]==B_P[1] ){
		if(A_P[0]==B_P[0]){
			return 0;
		};
		return ( A_P[0] > B_P[0] ?1:-1);
	};
	return ( A_P[1] > B_P[1] ?1:-1);
}
//减法
STT_64BIT sub_64BIT(const STT_64BIT  A,const STT_64BIT  B)
{
	register uint32_t * A_P=(uint32_t *)(&A);
	register uint32_t * B_P=(uint32_t *)(&B);
	register uint32_t ret[2];
	ret[0]= A_P[0] - B_P[0];
	ret[1]= A_P[1] - B_P[1] - (( ret[0] > A_P[0] ) ? 1 : 0) ; 
	return *((STT_64BIT *)ret);
}
//加法
STT_64BIT add_64BIT(const STT_64BIT  A,const STT_64BIT  B)
{
	register uint32_t * A_P=(uint32_t *)(&A);
	register uint32_t * B_P=(uint32_t *)(&B);
	register uint32_t ret[2];
	ret[0]= A_P[0] + B_P[0];
	ret[1]= A_P[1] + B_P[1] + (( ret[0] < A_P[0] ) ? 1 : 0) ; 
	return *((STT_64BIT *)ret);
}



static STT_64BIT  __div64_32(STT_64BIT *n, uint32_t base)
{
	STT_64BIT rem;
	STT_64BIT b  ;
	STT_64BIT res={0,0,0,0}, d = {1,0,0,0};
	uint32_t flag =0;
	uint32_t high = (*( ((uint32_t *)(n)) +1));
	rem = *n;
	(*( ((uint32_t *)(&(b)))   ))=base;
	if (high >= base) {
		high /= base;
		(*( ((uint32_t *)(&(res))) +1))=high;
		(*( ((uint32_t *)(&(rem))) +1)) -= (high*base);
	};
	while(((!((*( ((uint32_t *)(&(rem))) +1))==0 && (*( ((uint32_t *)(&(rem))) ))==0))&&
		((*( ((int32_t *)(&(rem))) +1)) >=0))
                    &&(cmp_64BIT(b,rem)<0) ){
		// b=add_64BIT(b,b);
		// d=add_64BIT(d,d);
		flag=((*( ((uint32_t *)(&(b)))  ))& (0X0001<<31) );
		(*( ((uint32_t *)(&(b)))+1)) <<= 1;
		(*( ((uint32_t *)(&(b)))  )) <<= 1;
		(*( ((uint32_t *)(&(b)))+1)) +=  (flag>>31) ;

		flag=((*( ((uint32_t *)(&(d)))  ))& (0X0001<<31) );
		(*( ((uint32_t *)(&(d)))+1)) <<= 1;
		(*( ((uint32_t *)(&(d)))  )) <<= 1;
		(*( ((uint32_t *)(&(d)))+1)) +=  (flag>>31) ;
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
/* 
 * unsigned 64位除法，需要的得到余数
 * Param - u64	: 被除数
 * Param - u32	: 除数
 * Param - u32* : 除后的余数
 * Return - u64	: 除后的结果
 */
STT_64BIT div_64BIT(STT_64BIT dividend, uint32_t divisor, STT_64BIT *remainder)
{
	if(remainder!=NULL){
		*remainder = __div64_32(&dividend, divisor);
	}else{
		__div64_32(&dividend, divisor);
	};
	return dividend;
}
//乘法
STT_64BIT mul_64BIT(const STT_64BIT  A,const STT_64BIT  B)
{
	register uint16_t * A_P=(uint16_t *)(&A);
	register uint16_t * B_P=(uint16_t *)(&B);
	register char j,i,k=3;
	register uint32_t ret[2]={0,0};
	register uint32_t * _A_P;
	for(j=0;j<=3 ;j++,k--){
		for(i=0;i<=k ;i++ ){
			register uint16_t    temp[5]={0,0,0,0};
			*((uint32_t *)(temp + i+j ))=(uint32_t)(A_P[j]) * (uint32_t)(B_P[i]);
			_A_P=(uint32_t *)(temp);
			ret[0]= _A_P[0] + ret[0];
			ret[1]= _A_P[1] + ret[1] + (( ret[0] < _A_P[0] ) ? 1 : 0) ;
		};
	};
	return *((STT_64BIT *)ret);
}
