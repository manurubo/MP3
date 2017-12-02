#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
int pos=0;

int llegir_numero(char *string)
{
    int numcaract=0;
    char caracters[12];
    int i;
    for(i=0;i<=12;i++)
    {
        caracters[i]='a';
    }

    while(string[pos] >= 48 && string[pos] <= 57)
    {
        caracters[numcaract]=string[pos];
        numcaract++;
        pos++;
    }
    //printf("%d\t", atoi(caracters));
    //printf("numero %d: ", atoi(numero));
    return atoi(caracters);
}
int multidiv (char *string, int llargada)
{
     //int i;
     int multidiv = 0;
     int parentesis = -1;
     int buf=pos;
     while( pos<llargada && parentesis<0){
            if(string[pos]==40) parentesis--;
            else if(string[pos]==41) parentesis++;
            if(string[pos]=='/' || string[pos]=='*') multidiv++;
            pos++;
     }
     pos = buf;
     return multidiv;
}

int sumaires (char *string, int llargada)
{
     int buf=pos;
     int sumaires = 0;
     int parentesis = -1;
     while( pos<llargada && parentesis<0){
            if(string[pos]==40) parentesis--;
            else if(string[pos]==41) parentesis++;
            if(string[pos]=='+' || string[pos]=='-') sumaires++;
            pos++;
     }
     pos=buf;
     return sumaires;
}

int processar(char *string)
{
    int resultat=0;
    int llargada=strlen(string)-1;
    int a = sumaires(string, llargada);
    printf("%d sumaires \t", a);
    int b = multidiv(string, llargada);
    printf("%d multidiv \t", b);
    while(string[pos] != '\n' && string[pos]!= EOF)
    {
        if(string[pos] == '(')
        {
            pos++;
            resultat=processar(string);

        }
        else if(string[pos]==')')
        {
            pos++;
            return resultat;
        }
        else if(string[pos] >= 48 && string[pos] <= 57){
            resultat = llegir_numero(string);
            //printf("%d\t", resultat);
        }
        else if(b>0 && (string[pos]=='/' || string[pos]=='*'))
        {
            switch(string[pos]){

            case '/':
                pos++; b--;
                int divisor;
                if (string[pos]=='('){
                    pos++;
                    divisor=processar(string);
                    //assert(divisor != 0);
                    if (divisor==0){
                        printf("error  ");
                        return divisor;
                    }

                    else
                        resultat=resultat/divisor;
                }

                else{
                    int divisor= llegir_numero(string);
                    //assert(divisor != 0);
                    if (divisor==0){
                        printf("error  ");
                        return divisor;
                    }
                    else
                        resultat=resultat/divisor;
                }

                break;

            case '*':
                pos++;b--;
                if (string[pos]=='('){
                    pos++;
                    resultat=resultat*processar(string);
                }
                else
                    resultat=resultat*llegir_numero(string);
                break;
            }
        }
        else if(b==0 && a>0 && (string[pos]=='+' || string[pos]=='-'))
            {
                 switch(string[pos]){
                     case '+':
                        pos++;a--;
                        if (string[pos]=='('){
                            pos++;
                            resultat=resultat+processar(string);
                        }
                        else
                            resultat=resultat+llegir_numero(string);
                        break;

                    case '-':
                        pos++;a--;
                        if (string[pos]=='('){
                            pos++;
                        resultat=resultat-processar(string);
                        }
                        else
                            resultat=resultat-llegir_numero(string);
                        break;
                 }
            }
        else if(b>0 && (string[pos]=='+' || string[pos]=='-') )
        {
            pos++;
            resultat=0;
        }
        //si ha acabado de hacer las multiplicaciones y divisiones, tendra que hacer las sumas i restas aunque haya llegado al final del string
        if(a>0 && (string[pos+1]=='\n' || string[pos+1]==EOF)) pos=0;

    }
    //caracter=string;
    //while()
    return resultat;
}


int parentesis(char *string, int llargada)
{
    int n=0; int obertura=0, tancament=0;
    while(n<llargada)
    {
        if(string[n] == 40)
            obertura++;
        else if(string[n] == 41)
            tancament++;
        n++;
    }
    if(obertura == tancament)
        return obertura;
    else
        return -1;
}

bool veinatgesimbols(char *string, int llargada)
{
    bool trobat = true; int i;
    for( i =0; i<llargada && trobat; i++){
        if(string[i] >= 48 && string[i] <= 57){
            if(string[i+1]==40) trobat = false;
        }
        else if( string[i]=='+' || string[i]=='-' || string[i]=='/' || string[i]=='*'){
            if( string[i+1]=='+' || string[i+1]=='-' || string[i+1]=='/' || string[i+1]=='*' || string[i+1] == 41) trobat = false;
        }

    }
    return trobat;
}

int simbols (char *string, int llargada)
{
    int n=0;
    int trobat=0;
    while(n<llargada && trobat!=-1)
    {
        if((string[n]<40 || string[n]>57) || string[n]==44 || string[n]==46)
            trobat=-1;
        n++;
    }
    return trobat;
}


int main()
{
    char string[256];
    FILE *archiu;
    archiu = fopen( "provesbones.txt" , "r" );
    fgets(string, 256, archiu);
    int resultat,llargada;
    bool veinatge;
    while( feof(archiu) == 0)
    {
        llargada=strlen(string)-1;
        resultat=simbols(string,llargada);
        printf("simbols: %d\t", resultat);
        resultat=parentesis(string,llargada);
        printf("parentesi: %d\t",resultat);
        veinatge=veinatgesimbols(string, llargada);
        if(veinatge) printf("Veinatge dels simbols correcte \t");
        else printf("Veinatge dels simbols incorrecte");


        printf("%d\t",llargada);

        if(simbols(string,llargada)!=-1 && parentesis(string,llargada)!=-1 && veinatge){
            pos=0;
            printf("resultat: %d\t\t",processar(string));
        }
        printf("%s",string);
        fgets(string, 256, archiu);

    }
    fclose(archiu);

    return 0;
}
