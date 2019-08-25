#include <stdio.h>




int main(){
    
    FILE *f = fopen("HighScore.txt","a");
    
    int a = 10;
    fprintf(f,"%d ",a);
    
    fclose(f);
    
    int b;
    f = fopen("HighScore.txt","r");
    while(fscanf(f,"%d",&b) != EOF){
        printf("%d\n",b);
    }
    
    fclose(f);
    
    
    return 0;
}
