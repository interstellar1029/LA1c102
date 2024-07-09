/*
 * strncmp.c
 */

#include <string.h>

int strncmp(const char *s1, const char *s2, size_t n)
{
    if (n == 0)
        return 0;
        
    // initial
    while (n-- != 0 && *s1 == *s2)
    {
        if (n == 0 || *s1 == '\0')
            break;
        s1++;
        s2++;
    }
    // initial
    
    // my
	//while (--n && *s1 && *s1 == *s2) // ���ַ�����Ҳ�Ϊ��\0��ʱ�Ƚ��¸��ַ���ֱ�� n = 0 ����
	//{
		//s1++;
		//s2++;
    //}
    // my

    return (*(unsigned char *) s1) - (*(unsigned char *) s2);
}

