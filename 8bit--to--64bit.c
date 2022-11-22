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
extern STT_64BIT div_64BIT(STT_64BIT rem, uint32_t base, STT_64BIT *remainder);

void is_cpy(void* x,void* y,uint8_t __i__) 
{           
	 do{                    
	 	((uint8_t *)x)[__i__]=((uint8_t *)y)[__i__];  
	 }while(__i__--);       
}
//比较
char is_cmp( void* A_P, void* B_P,uint8_t i)
{
	do{
		if( ((uint8_t *)A_P)[i]==((uint8_t *)B_P)[i]){
			continue;
		};
		return (((uint8_t *)A_P)[i]>((uint8_t *)B_P)[i]?1:-1);
	}while(i--);
	return 0;
}
//比较
char cmp_64BIT( STT_64BIT* A_P, STT_64BIT* B_P)
{
	return is_cmp(  A_P,  B_P,7);
}
//减法
STT_64BIT sub_64BIT( STT_64BIT* A_P, STT_64BIT* B_P)
{
	 uint8_t ret[8];
	register uint8_t i=7,j=6;
	do{
		ret[i]= ((uint8_t *)A_P)[i]-((uint8_t *)B_P)[i]-
		        ( ((uint8_t *)A_P)[j]>((uint8_t *)B_P)[j]?1:0);
		i--;
	}while(--j);
	ret[0]= ((uint8_t *)A_P)[0]-((uint8_t *)B_P)[0];
	return *((STT_64BIT *)ret);
}
//加法
STT_64BIT add_64BIT( STT_64BIT* A_P, STT_64BIT* B_P)
{
	 uint8_t ret[8];
	register uint8_t i=0,j=1;
	ret[0]= ((uint8_t *)A_P)[0]+((uint8_t *)B_P)[0];
	do{
		ret[j]= ((uint8_t *)A_P)[j]+((uint8_t *)B_P)[j]+
		        ( ret[i]<((uint8_t *)B_P)[i]?1:0);
		i++;j++;
	}while(j<=7);
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
	register STT_64BIT  b     = {0,0,0,0};
	register STT_64BIT  res   = {0,0,0,0};
	register STT_64BIT  d     = {1,0,0,0};
	register uint32_t *     res_P =(uint32_t *)(&res);
	register uint32_t *	rem_P =(uint32_t *)(&rem);
	register uint32_t *	b_P   =(uint32_t *)(&b);
	register uint32_t *	d_P   =(uint32_t *)(&d);
	{
		register uint32_t    high  =(*( ((uint32_t *)(&rem)) +1));
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
STT_64BIT mul_64BIT(STT_64BIT*  A_P, STT_64BIT*  B_P)
{
	register uint8_t j,k=7;
	uint8_t ret[8]={0,0,0,0,0,0,0,0};
	if( cmp_64BIT( A_P, B_P)>0 ){
		register STT_64BIT* temp_P=A_P;
		A_P=B_P;
		B_P=temp_P;
	};
	for(j=0;j<=7 ;j++,k--){
		if( ((uint8_t *)A_P)[j] == 0 ){
			continue;
		};
		{
			register uint8_t i;
			for(i=0; i<= k;i++ ){
			    if( ((uint8_t *)B_P)[i] == 0){
					continue;
				};
				{
					uint8_t    temp[9]={0,0,0,0,0,0,0,0};
					*((uint16_t *)(temp + i+j ))= ((uint8_t *)A_P)[j] * ((uint8_t *)B_P)[i];
					*((STT_64BIT *)ret)=add_64BIT( (STT_64BIT *)temp, (STT_64BIT *)ret );
				};
			};
		};
	};
	return *((STT_64BIT *)ret);
}
/*

{

do{
	Q__[i__]<<=1 ;
	i__--;
	if(Q__[i__] & (1<<7) ){Q__[i__+1]++;};
}while(i__>1);
Q__[0]<<=1;
}


STT_64BIT  div_64BIT(STT_64BIT* rem, uint32_t * base, STT_64BIT *remainder)
{
	 uint8_t res_P[8]={0,0,0,0,0,0,0,0};
	 uint8_t b_P[8]={0,0,0,0,0,0,0,0};
	 uint8_t d_P[8]={1,0,0,0,0,0,0,0};
	 uint8_t rem_P[8];
	 is_cpy(rem_P,rem,7);

	{
		uint8_t high[4];
		is_cpy(high,rem_P+4,3);
		is_cpy(b_P,base,3);
		if (is_cmp( high,base,3)>=0) {
			*high /= *base;
			is_cpy(res_P+4,high,3);
			((uint32_t *)rem_P)[1]  -= ( ((uint32_t *)high)[0] * ((uint32_t *)base)[0] );
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

 */
