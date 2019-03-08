/* lab 01: somador de floats
 * 
 * Nome: Ana Marina Araujo Santos		RA:134911
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*determina se é digito*/
int verdig(char c){
  if ((c >= '0') && (c <= '9')) return 1;
  else return 0;
}

/*determina se é espaço, quebra de linha ou tabulação*/
int veresp(char c){
  if ((c == ' ')||(c == '\n')||(c == '\t')) return 1;
  else return 0;
}

/*determina se é especificamente um ponto*/
int verflo(char c){
  if ((c == '.')) return 1;
  else return 0;
}

/*esta fução sera utilizada para zerar ou inicializar vetores de caracteres utilizados no programa*/
void zeraouinicia(char *vet,int n){
int i;
  for (i=0;i<n;i++){
    vet[i]='\0';
  }
}

/*define a estrutura nescessaria para guardar o estado da maquina de estados*/
typedef struct {
  int estado_atual;
} maquina, *MAQ;

/*função que determina o estado da maquina e o retorna*/
int lookfloats(void *minha_maquina, char dig_ava,char dig_ant) {
  MAQ soma_float = (MAQ) minha_maquina;   
   switch (soma_float->estado_atual) {
    case 0://estado de leitura do primeiro caracter da string, ou de um bloco de caracters separados por espaço.
      if (verdig(dig_ava)) {
        soma_float->estado_atual = 2;// se é digito passa para o estado 2.
      } else 
      soma_float->estado_atual = 1;//se não vai para o estado 1.
      break;
   
  /*este estado é um estado no qual esperamos especeficamente por um espaço ou tabulação ou \n
   -casa se tenha um digito junto a uma sequencia de caracteres
   -no caso de digito.(não digito)
   -se ter um não digito(ou como primeiro caracter da string de entrada ou no inicio de um bloco separados por espaço)
   */    
    case 1:
      if (veresp(dig_ava)) {
        soma_float->estado_atual = 0;// temos um espaço -que dara inicio a leitura de um novo bloco de caracteres (estado 0)
      } else 	
      soma_float->estado_atual = 1;//permanece neste estado e procura por um espaço. 
      break;
         
    case 2:
      if (verdig(dig_ava)) {
        soma_float->estado_atual = 2;//se é uma sequencia de digitos validas ele permnesse no estado 2
      }else if(veresp(dig_ava)){
	soma_float->estado_atual = 4;//se encontra um espaço (indicando um numero valido) vai para o estado 4
      }else if(verflo(dig_ava)){
	soma_float->estado_atual = 3;//se encontra um ponto vai para o estado 3
      }else
      soma_float->estado_atual = 1;//vai ao estado 1;
      break;
    
    /*
     este estado é especificamente um estado onde temos (digito.)
     */
    case 3:
      if (verdig(dig_ava)) {
        soma_float->estado_atual = 3;//se apos digito. é outro digito fica no estado 3
      }else if(veresp(dig_ava) && verdig(dig_ant)){//vai para o estado 4 se encontra espaço sucessor a um digito
	soma_float->estado_atual = 4;
      }else
      soma_float->estado_atual = 1;//vai para o estado 1.
      break;
	
     /*
     este estado é especificamente o estado em que se encontrou um numero valido para a soma.
     Neste caso o numero sera somado, mas ao entrarmo aqui ja temos o proximo digito a ser avaliado. 
     */  
    case 4:
      if (verdig(dig_ava)) {
        soma_float->estado_atual = 2;//se for digito vai para o estado 2
      }else  if(veresp(dig_ava)){
	soma_float->estado_atual = 0;//se houver espaço em sequencia vai para o estado 0(novo bloco).
      }else
      soma_float->estado_atual = 1;//se não vai para o estado um.

    break;
  }
  return soma_float->estado_atual;
}


int main() {
  //inicializa oas variaveis e vetores nescessarios
  char caracter;
  char string[180];//armazena a entrada
  int ind=0;int i=0;
  int it=0;
  char temp[30];//armazena um numero
  float soma=0;//armazena a soma
  float e=0;//armazena o numero convertido em float
   
  //inicializa os vetores nescessarios 
  zeraouinicia(string,180);
  zeraouinicia(string,30);
  
  //le a entrada
  do {
    caracter=getchar();
    string[ind] = caracter;
    ind++;
  } while (caracter != '\n');
  
  //cria a maquina de fato.
  maquina srcfloat;
  srcfloat.estado_atual = 0;
  ind=0;
  
  //este loop realiza a avaliação da entrada 
  while (string[ind] != '\0') {
    lookfloats((void*) (&srcfloat), string[ind],string[ind-1]);//chama a função que gerencia os estados.
    //se nos estados 2 ou 3 temos numero candidato a ser valido e devemos ir armazenando no vetor temp
    if(srcfloat.estado_atual==2||srcfloat.estado_atual==3){
      temp[it]=string[ind];it++;
    }else if(srcfloat.estado_atual==0||srcfloat.estado_atual==1){//se o numero se torna invalido zeramos o vetor temp
      zeraouinicia(temp,30);
      it=0;
    }else{//mas se o numero é valido nos convertemos temp em float e somamos e logo em seguida zeramos temp para receber novo numero 
      e=atof(temp);
      soma=soma+e;
      zeraouinicia(temp,30);
      it=0;
    }

    ind++;
  }
  
  //avalia se é inteiro ou float ne determina a forma como a saida deve ser impressa.
  float res=0;
  res=soma-((int)soma);
  if(res==0){
   printf("%.0f\n",soma);
  }else	
  printf("%.1f\n",soma);

  return 0;
}
