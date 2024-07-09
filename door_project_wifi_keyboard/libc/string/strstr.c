
/*
 * strstr.c
 */

#include <string.h>

char* strstr ( const char* const haystack, const char* const needle )
{
	//查找子串末尾地址
	register const char* const needle_end = strchr ( needle, '\0' );

	//被查找子串末尾地址
	register const char* const haystack_end = strchr ( haystack, '\0' );

	//查找子串长度
	register const size_t needle_len = needle_end - needle;

	//实现原理字符串从查找子串尾到头比较，提供被查找开始地址使用
	register const size_t needle_last = needle_len - 1;

	//定义被查找子串开始地址
	register const char* begin;

	//判断查找子串为0或者查找子串比被查找子串短，直接返回
	if ( needle_len == 0 )
	{
		return ( char* ) haystack;    /* ANSI 4.11.5.7, line 25.  */
	}
	if ( ( size_t ) ( haystack_end - haystack ) < needle_len )
	{
		return NULL;
	}

	//开始比较，比较方式从被查找子串最后一个字符向前比较，注意被查找子串的开始地址，比较有差异，增加被查找地址在逐个字符从后向前比较
	for ( begin = &haystack[needle_last]; begin < haystack_end; ++begin )
	{
		//查找子串的最后一个字符
		register const char* n = &needle[needle_last];

		//被查找子串的开始地址，for中地址逐次+1
		register const char* h = begin;

		do {
			if ( *h != *n ) //字符比较不同，直接结束，执行下一次for循环
			{
				goto loop;    /* continue for loop */
			}
		} while ( --n >= needle && --h >= haystack ); //继续向前比较查找字符

		return ( char* ) h;

loop:
		;
	}

	return NULL;
}

