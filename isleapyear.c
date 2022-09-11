#include <stdio.h>
int main()
{
    int y;

    printf("Enter year: ");
    scanf("%d",&y);

    if(y % 4 == 0)
    {

        if( y % 100 == 0)
        {
            if ( y % 400 == 0)
                printf("%d was a Leap Year", y);
            else
                printf("%d was not a Leap Year", y);
        }
        else
            printf("%d was a Leap Year", y );
    }
    else
        printf("%d was not a Leap Year", y);

    return 0;
}
