
/*
 * strstr.c
 */

#include <string.h>

char* strstr ( const char* const haystack, const char* const needle )
{
	//�����Ӵ�ĩβ��ַ
	register const char* const needle_end = strchr ( needle, '\0' );

	//�������Ӵ�ĩβ��ַ
	register const char* const haystack_end = strchr ( haystack, '\0' );

	//�����Ӵ�����
	register const size_t needle_len = needle_end - needle;

	//ʵ��ԭ���ַ����Ӳ����Ӵ�β��ͷ�Ƚϣ��ṩ�����ҿ�ʼ��ַʹ��
	register const size_t needle_last = needle_len - 1;

	//���屻�����Ӵ���ʼ��ַ
	register const char* begin;

	//�жϲ����Ӵ�Ϊ0���߲����Ӵ��ȱ������Ӵ��̣�ֱ�ӷ���
	if ( needle_len == 0 )
	{
		return ( char* ) haystack;    /* ANSI 4.11.5.7, line 25.  */
	}
	if ( ( size_t ) ( haystack_end - haystack ) < needle_len )
	{
		return NULL;
	}

	//��ʼ�Ƚϣ��ȽϷ�ʽ�ӱ������Ӵ����һ���ַ���ǰ�Ƚϣ�ע�ⱻ�����Ӵ��Ŀ�ʼ��ַ���Ƚ��в��죬���ӱ����ҵ�ַ������ַ��Ӻ���ǰ�Ƚ�
	for ( begin = &haystack[needle_last]; begin < haystack_end; ++begin )
	{
		//�����Ӵ������һ���ַ�
		register const char* n = &needle[needle_last];

		//�������Ӵ��Ŀ�ʼ��ַ��for�е�ַ���+1
		register const char* h = begin;

		do {
			if ( *h != *n ) //�ַ��Ƚϲ�ͬ��ֱ�ӽ�����ִ����һ��forѭ��
			{
				goto loop;    /* continue for loop */
			}
		} while ( --n >= needle && --h >= haystack ); //������ǰ�Ƚϲ����ַ�

		return ( char* ) h;

loop:
		;
	}

	return NULL;
}

