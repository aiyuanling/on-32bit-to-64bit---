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
        return   (( A_P[1]== B_P[1] ) ?
		 (( A_P[0]== B_P[0] ) ? 0 : 
		 (( A_P[0] > B_P[0] ) ? 1 : -1))
	                                  :
	         (( A_P[1] > B_P[1] ) ? 1 : -1));
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
	};
	return res;
}
//乘法
STT_64BIT mul_64BIT(const STT_64BIT  A,const STT_64BIT  B)
{
	register uint16_t * A_P=(uint16_t *)(&A);
	register uint16_t * B_P=(uint16_t *)(&B);
	register uint8_t j,k=3;
	register uint32_t ret[2]={0,0};
	if( ((uint32_t *)A_P)[1] > ((uint32_t *)B_P)[1] ){
		register uint16_t * temp_P=A_P;
		A_P=B_P;
		B_P=temp_P;
	};
	for(j=0;j<=3 ;j++,k--){
		if(A_P[j] == 0 ){
			continue;
		};
		{
			register uint8_t i=0;
			for(; i<= k;i++ ){
			    if( B_P[i] == 0){
					continue;
				};
				{
					register uint32_t * _A_P;
						{
							register uint16_t    temp[5]={0,0,0,0};
							*((uint32_t *)(temp + i+j ))=(uint32_t)(A_P[j]) * (uint32_t)(B_P[i]);
							_A_P=(uint32_t *)(temp);		
						};
					ret[0] += _A_P[0] ;
					ret[1] += _A_P[1] + (( ret[0] < _A_P[0] ) ? 1 : 0) ;				
				};
			};
		};
	};
	return *((STT_64BIT *)ret);
}
