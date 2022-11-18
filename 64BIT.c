
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

// //3212/X/2=41/2
// // 若M为偶数
// // M*N=(M/2)*2N
// // 若M为奇数
// // M*N=(M/2)*2N+N
// STT_64BIT mul_64BIT(const STT_64BIT  A,const STT_64BIT  B)
// {
// 	STT_64BIT *  m,n,t;
// 	STT_64BIT ret={0,0,0,0};
//  if(!( ((*( ((uint32_t *)(&(A))) +1)) >0 || (*( ((uint32_t *)(&(A))) )) >0)&&
//   	   ((*( ((uint32_t *)(&(B))) +1)) >0 || (*( ((uint32_t *)(&(B))) )) >0) )
//   ){
//  	return ret;
//  };
// if( (*( ((uint32_t *)(&(A))) +1))>(*( ((uint32_t *)(&(B))) +1)) 
//        ){
// 	m=&A;
// 	n=&B;
// }else{
// 	m=&B;
// 	n=&A;
// };

//   while ((*( ((uint32_t *)((m))) )) != 1 || (*( ((uint32_t *)((m))) +1))>0  ) {
//     if (m & 1) {
//       t += n;
//     }
//     m >>= 1;
//     n <<= 1;
//   }



// }

// /* 
//  * unsigned 64位除法，需要的得到余数
//  * Param - u64	: 被除数
//  * Param - u32	: 除数
//  * Param - u32* : 除后的余数
//  * Return - u64	: 除后的结果
//  */
// uint64_t div_u64_rem(uint64_t dividend, uint32_t divisor, uint32_t *remainder)
// {
// 	*remainder = __div64_32(dividend, divisor);
// 	return dividend;
// }

// uint32_t  __div64_32(uint64_t *n, uint32_t base)
// {
// 	uint64_t rem = *n;
// 	uint64_t b = base;
// 	uint64_t res, d = 1;
// 	uint32_t high = rem >> 32;

// 	 Reduce the thing a bit first 
//     /* 先算高32位，再算低32位，利用(a+b)/c = a/c + b/c的原理*/
// 	res = 0;
// 	if (high >= base) {
//         /* 当被除数的高32位大于除数时，利用(a*b)/c =a / c * b的除法定理  */
// 		high /= base;
// 		res = (uint64_t) high << 32;
// 		rem -= (uint64_t) (high*base) << 32;
// 	}
   
//     /*这里为什么不直接 rem / b 来得到商和余数？因为这时候的rem还是大于32位的 */
// 	while ((int64_t)b > 0 && b < rem) {
//         /* 
//          通过循环倍增算出被除数由多少个除数相加得到，比如被除数是20，除数是3，
//          第一次循环b = 3,d = 1,rem = 20,第二次循环b = 6,d = 2,rem = 20,
//          第三次循环b = 12, d = 4, rem = 20,第四次循环 b = 24, d = 8 , rem = 20,循环结束 
//          n次循环
//          b = b*(2^(n-1))
//          d = 2^(n - 1) 
//         */
// 		b = b+b;
// 		d = d+d;
// 	}

// 	do {
//         /*
//          上面的循环结束后，b = 24, d = 8, rem = 20进入这个循环体
//          第二次循环b = 12, d = 4, rem = 20
//          第三次循环b = 6 , d = 2, rem = (20 - 12) = 8, 商res += 4 = 4；
//          第四次循环b = 3 , d = 1, rem = (8 - 6) = 2, 商res += 2 = 6；
//          第五次循环b = 1 , d = 0, rem = 2, 商res = 6。循环结束
//          就这样算出了20 / 3 = 6……2的结果
//         */
// 		if (rem >= b) {
// 			rem -= b;
// 			res += d;
// 		}
// 		b >>= 1;
// 		d >>= 1;
// 	} while (d);

// 	*n = res;
// 	return rem;
// }
