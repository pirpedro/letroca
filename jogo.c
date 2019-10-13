/*
Bibliotecas
*/
#include<stdio.h>
#include<allegro.h>
#include<time.h>
#include<string.h>
#include<stdlib.h>
/*
Constantes
*/
#define BIT_COLOR 16
#define RESOLUCAO_HOR 800
#define RESOLUCAO_VER 600
#define MAX_LETRAS 17
#define LARGURA_LETRA 50
#define ALTURA_LETRA 50
#define COR_DAS_LETRAS 0
#define	LINHA_PALAVRA_ORIGINAL 450
#define LINHA_PALAVRA_FORMADA 350
#define TEMPO_DE_JOGO 180
/*
Variaveis Globais
*/
int game_over;
int pontos;
int tempo;
int qtdadepalavras;
int proxima_fase;
/*
Funções
*/
int inicializaAllegro();
int finalizaAllegro();
int inicializaGrafico();
int aberturaJogo();
int fechaduraJogo();
void telaRecordes(int acao);
void ajuda();
int compara(char *v1,char *v2);
void minuscula(char *v);
void maiuscula(char *v);
int verificaLetra(char letra,char *original,char *atual);
void embaralhaPalavra(char *v);
void achei(void);
void acheiDenovo(void);
void naoAchei(void);
void avisoMaior3(void);
int jogar(void);
void desenhaTela(char *palavra,int *achadas,char **matriz,int n);
void letraA(int x,int y);
void letraB(int x,int y);
void letraC(int x,int y);
void letraD(int x,int y);
void letraE(int x,int y);
void letraF(int x,int y);
void letraG(int x,int y);
void letraH(int x,int y);
void letraI(int x,int y);
void letraJ(int x,int y);
void letraL(int x,int y);
void letraM(int x,int y);
void letraN(int x,int y);
void letraO(int x,int y);
void letraP(int x,int y);
void letraQ(int x,int y);
void letraR(int x,int y);
void letraS(int x,int y);
void letraT(int x,int y);
void letraU(int x,int y);
void letraV(int x,int y);
void letraX(int x,int y);
void letraZ(int x,int y);
void apagaLetra(int x,int y);
void desenhaLetra(char l,int x,int y);
void incrementa_tempo (void);
void contaPalavras(void);
int contaPalavrasPossiveis(char *v);
void verificaPalavrasPossiveis(char *v,char **matriz);

int main(void)
{
	inicializaAllegro();
	inicializaGrafico();
	srand(time(NULL));
	show_mouse(NULL);
	game_over=0;
	if (aberturaJogo())
	{	
		while(1)
		{
			while(!game_over)
			{
				proxima_fase=0;
				if (jogar()==2)
				{
					game_over=1;
				}
			}
			if (fechaduraJogo())
			{
				telaRecordes(1);
				telaRecordes(0);
			}
			pontos=0;
			if(!aberturaJogo())
			{
				break;
			}
			game_over=0;
		}
	}
	finalizaAllegro();
	return 0;
}
END_OF_MAIN();

int inicializaAllegro(void)
{
	allegro_init(); 
	install_keyboard();
	install_mouse();
	install_timer(); 	
	return 0;
}

int finalizaAllegro(void)
{
	allegro_exit();	
	return 0;
}

int inicializaGrafico()
{
		set_color_depth(BIT_COLOR);
		set_gfx_mode(GFX_AUTODETECT,RESOLUCAO_HOR,RESOLUCAO_VER,0,0);
		return 0;
	
}

int aberturaJogo()
{
	BITMAP *abertura;
	PALLETE pal;
	abertura=load_bmp("./GRAFICOS/abertura.bmp",pal);
	set_pallete(pal);
	
	while(1)
	{
		show_mouse(NULL);
		blit(abertura,screen,0,0,0,0,RESOLUCAO_HOR,RESOLUCAO_VER);
		show_mouse(screen);
		while (!keypressed() && !(mouse_b & 1));
		if (key[KEY_ENTER] || key[KEY_J] || ((mouse_b & 1) && mouse_x>259 && mouse_x<391 && mouse_y>386 && mouse_y<421))
		{	
			clear_keybuf();
			show_mouse(NULL);
			blit(abertura,screen,0,0,0,0,RESOLUCAO_HOR,RESOLUCAO_VER);
			return 1;
		}
		else if (key[KEY_R] || ((mouse_b & 1) && mouse_x>407 && mouse_x<539 && mouse_y>386 && mouse_y<421))
		{
			telaRecordes(0);
		}
		else if (key[KEY_A] || ((mouse_b & 1) && mouse_x>334 && mouse_x<466 && mouse_y>438 && mouse_y<473))
		{
			ajuda();
		}
		else if (key[KEY_ESC] || key[KEY_S] || ((mouse_b & 1) && mouse_x>334 && mouse_x<466 && mouse_y>492 && mouse_y<527))
		{
			return 0;
		}
		mouse_b=0;
		clear_keybuf();
	}
	
	return 0;
}

int fechaduraJogo()
{
	FILE *p;
	int recorde;
	int i;
	char vetor[5];
	BITMAP *fechadura;
	BITMAP *temp;
	PALLETE pal;
	temp=create_bitmap(RESOLUCAO_HOR,RESOLUCAO_VER);
	fechadura=load_bmp("./GRAFICOS/gameover.bmp",pal);
	set_pallete(pal);
	show_mouse(NULL);
	blit(fechadura,screen,0,0,0,0,RESOLUCAO_HOR,RESOLUCAO_VER);
	show_mouse(screen);
	rest(1500);
	p=fopen("./DADOS/recordes.txt","r");
	for (i=0;i<10;i++)
	{
		fscanf(p,"%d %s",&recorde,vetor);
	}
	if (pontos > recorde)
	{
		return 1;	
	}
	else
	{
		return 0;
	}
}

void telaRecordes(int acao)
{
	BITMAP *recordes;
	PALLETE pal;
	struct _jogador{
		char iniciais[3];	
		int ponto;
	} jogadores[10],novo,aux;
	FILE *p;
	int i,j;
	recordes=load_bmp("./GRAFICOS/recordes.bmp",pal);
	p=fopen("./DADOS/recordes.txt","r");
	text_mode(-1);
	for(i=0;i<10;i++)
	{
		fscanf(p,"%d %s",&jogadores[i].ponto,jogadores[i].iniciais);
		maiuscula(jogadores[i].iniciais);
		textprintf(recordes,font,500,175+25*i,145,"%d",jogadores[i].ponto);
		textprintf(recordes,font,280,175+25*i,145,"%s",jogadores[i].iniciais);
	}
	fclose(p);
	set_pallete(pal);
	show_mouse(NULL);
	blit(recordes,screen,0,0,0,0,RESOLUCAO_HOR,RESOLUCAO_VER);
	show_mouse(screen);
	clear_keybuf();
	mouse_b=0;
	if (acao)
	{
		text_mode(-1);
		novo.iniciais[0]='_';
		novo.iniciais[1]='_';
		novo.iniciais[2]='_';
		textprintf(screen,font,280,500,COR_DAS_LETRAS,"Entre com seu nome: %c %c %c <ENTER>",novo.iniciais[0],novo.iniciais[1],novo.iniciais[2]);
		i=0;
		while (i<4)
		{
			while (!keypressed());
			novo.iniciais[i]=readkey() & 0xFF;
			maiuscula(&novo.iniciais[i]);
			if (key[KEY_BACKSPACE])
			{
				if (i>0)
				{
					novo.iniciais[i]='_';
					i--;
					novo.iniciais[i]='_';
				}
				show_mouse(NULL);
				blit(recordes,screen,0,0,0,0,RESOLUCAO_HOR,RESOLUCAO_VER);
				show_mouse(screen);
				textprintf(screen,font,280,500,COR_DAS_LETRAS,"Entre com seu nome: %c %c %c <ENTER>",novo.iniciais[0],novo.iniciais[1],novo.iniciais[2]);
			}
			else if (novo.iniciais[i]>='A' && novo.iniciais[i]<='Z' && i<3)
			{
				show_mouse(NULL);
				blit(recordes,screen,0,0,0,0,RESOLUCAO_HOR,RESOLUCAO_VER);
				show_mouse(screen);
				textprintf(screen,font,280,500,COR_DAS_LETRAS,"Entre com seu nome: %c %c %c <ENTER>",novo.iniciais[0],novo.iniciais[1],novo.iniciais[2]);
				i++;
			}
			else if (key[KEY_ENTER])
			{
				clear_keybuf();
				mouse_b=0;
				break;
			}
		}
		clear_keybuf();
		if (i==0)
		{
			novo.iniciais[0]='-';
			novo.iniciais[1]='-';
			novo.iniciais[2]='-';
			i=3;
		}
		novo.iniciais[i]='\0';
		maiuscula(novo.iniciais);
		novo.ponto=pontos;
		for (i=0;i<10;i++)
		{
			fscanf(p,"%d",&jogadores[i].ponto);
			fgets(jogadores[i].iniciais,5,p);
		}
		jogadores[9]=novo;
		for (i=0;i<10;i++)
		{
			for (j=0;j<9-i;j++)	
			{
				if (jogadores[j].ponto < jogadores[j+1].ponto)
				{
					aux=jogadores[j];
					jogadores[j]=jogadores[j+1];
					jogadores[j+1]=aux;
				}
				
			}
		}
		fclose(p);	
		p=fopen("./DADOS/recordes.txt","w");
		for (i=0;i<10;i++)
		{
			fprintf(p,"%d ",jogadores[i].ponto);
			fprintf(p,"%s\n",jogadores[i].iniciais);
		}	
		fclose(p);
	}
	else
	{
		while (!key[KEY_ENTER] && !((mouse_b & 1) && mouse_x>355 && mouse_x<442 && mouse_y>539 && mouse_y<583));
		clear_keybuf();
	}
}
void ajuda()
{
	BITMAP *ajuda;
	BITMAP *temp;
	PALLETE pal;
	temp=create_bitmap(RESOLUCAO_HOR,RESOLUCAO_VER);
	show_mouse(NULL);
	blit(screen,temp,0,0,0,0,RESOLUCAO_HOR,RESOLUCAO_VER);
	ajuda=load_bmp("./GRAFICOS/ajuda.bmp",pal);
	draw_sprite(screen,ajuda,(RESOLUCAO_HOR-ajuda->w)/2,(RESOLUCAO_VER-ajuda->h)/2);
	show_mouse(screen);
	clear_keybuf();
	while (!key[KEY_ENTER] && !((mouse_b & 1) && mouse_x>541 && mouse_x<589 && mouse_y>446 && mouse_y<475));
	show_mouse(NULL);
	blit(temp,screen,0,0,0,0,RESOLUCAO_HOR,RESOLUCAO_VER);
	show_mouse(screen);
	clear_keybuf();
}


void maiuscula(char *v)
{
	int i;
	for(i=0;v[i];i++)
	{
		if (v[i]>='a'&&v[i]<='z')
		{
			v[i]=v[i]+'A'-'a';
		}
	}
}
void minuscula(char *v)
{
	int i;
	for(i=0;v[i];i++)
	{
		if (v[i]>='A'&&v[i]<='Z')
		{
			v[i]=v[i]-'A'+'a';
		}
	}
}
int compara(char *v1,char *v2)
{
	int i;
	if (strlen(v1)!=strlen(v2)) return 0;
	for (i=0;v1[i];i++)
	{
		if (v1[i]!=v2[i])
		{
			return 0;
		}
	}
	return 1;
}

int verificaLetra(char letra,char *original,char *atual)
{
	int i,contOriginal=0,contAtual=0;
	for (i=0;i<strlen(original);i++)
	{
			if (original[i]==letra) contOriginal++;
	}
	for (i=0;i<strlen(atual);i++)
	{
			if (atual[i]==letra) contAtual++;
	}
	if (contOriginal<=contAtual)
	{
		return 0;	
	}
	else
	{
		return 1;	
	}
}
void embaralhaPalavra(char *v)
{
	int n[MAX_LETRAS];	
	int i,j,cont;
	char u[MAX_LETRAS];
	i=0;
	while (i<strlen(v))
	{
		cont=0;
		n[i]=rand()%strlen(v);
		for (j=0;j<i;j++)
		{
			if (n[i]!=n[j])	
			{
				cont++;
			}
		}	
		if (cont==i)	i++;
	}
	for (i=0;i<strlen(v);i++)
	{
		u[i]=v[n[i]];
	}
	for (i=0;i<strlen(v);i++)
	{
		v[i]=u[i];
	}
	
}

void achei(void)
{
	BITMAP *achar;
	BITMAP *temp;
	temp=create_bitmap(RESOLUCAO_HOR,RESOLUCAO_VER);
	show_mouse(NULL);
	blit(screen,temp,0,0,0,0,RESOLUCAO_HOR,RESOLUCAO_VER);
	PALLETE pal;
	achar=load_bmp("./GRAFICOS/correto.bmp",pal);
	set_pallete(pal);
	draw_sprite(screen,achar,(RESOLUCAO_HOR-achar->w)/2,(RESOLUCAO_VER-achar->h)/2);
	show_mouse(screen);
	rest(1500);
	show_mouse(NULL);
	blit(temp,screen,0,0,0,0,RESOLUCAO_HOR,RESOLUCAO_VER);
	show_mouse(screen);
	destroy_bitmap(temp);
	destroy_bitmap(achar);
}

void acheiDenovo(void)
{
	BITMAP *achar;
	BITMAP *temp;
	temp=create_bitmap(RESOLUCAO_HOR,RESOLUCAO_VER);
	show_mouse(NULL);
	blit(screen,temp,0,0,0,0,RESOLUCAO_HOR,RESOLUCAO_VER);
	PALLETE pal;
	achar=load_bmp("./GRAFICOS/denovo.bmp",pal);
	set_pallete(pal);
	draw_sprite(screen,achar,(RESOLUCAO_HOR-achar->w)/2,(RESOLUCAO_VER-achar->h)/2);
	show_mouse(screen);
	rest(1500);
	show_mouse(NULL);
	blit(temp,screen,0,0,0,0,RESOLUCAO_HOR,RESOLUCAO_VER);
	show_mouse(screen);
	destroy_bitmap(temp);
	destroy_bitmap(achar);
}

void naoAchei(void)
{
	BITMAP *naoachar;
	BITMAP *temp;
	temp=create_bitmap(RESOLUCAO_HOR,RESOLUCAO_VER);
	show_mouse(NULL);
	blit(screen,temp,0,0,0,0,RESOLUCAO_HOR,RESOLUCAO_VER);
	PALLETE pal;
	naoachar=load_bmp("./GRAFICOS/incorreto.bmp",pal);
	set_pallete(pal);
	draw_sprite(screen,naoachar,(RESOLUCAO_HOR-naoachar->w)/2,(RESOLUCAO_VER-naoachar->h)/2);
	show_mouse(screen);
	rest(1500);
	show_mouse(NULL);
	blit(temp,screen,0,0,0,0,RESOLUCAO_HOR,RESOLUCAO_VER);
	show_mouse(screen);
	destroy_bitmap(temp);
	destroy_bitmap(naoachar);
}

void avisoMaior3(void)
{
	BITMAP *aviso;
	PALLETE pal;
	aviso=load_bmp("./GRAFICOS/maior3.bmp",pal);
	set_pallete(pal);
	BITMAP *temp;
	temp=create_bitmap(RESOLUCAO_HOR,RESOLUCAO_VER);
	show_mouse(NULL);
	blit(screen,temp,0,0,0,0,RESOLUCAO_HOR,RESOLUCAO_VER);
	draw_sprite(screen,aviso,(RESOLUCAO_HOR-aviso->w)/2,(RESOLUCAO_VER-aviso->h)/2);
	show_mouse(screen);
	rest(1500);
	show_mouse(NULL);
	blit(temp,screen,0,0,0,0,RESOLUCAO_HOR,RESOLUCAO_VER);
	show_mouse(screen);
	destroy_bitmap(temp);
	destroy_bitmap(aviso);
}
int jogar(void)
{
	BITMAP *temp;
	BITMAP *tela;
	tela=create_bitmap(50,20);
	temp=create_bitmap(50,20);
	int inicio;
	int *achadas;								//mallocar com possiveis
	char **matriz;								//mallocar com tds
	char palavra[MAX_LETRAS];	//palavra digitada pelo usuario
	char novaPalavra[MAX_LETRAS]; // palavra sorteada embaralhada
	int i,j;	//contador
	int palavraSorteada;	//numero da palavra sorteada pelo random
	char letra;	//letra digitada pelo usuario
	FILE *arquivo;	//arquivo com as palavras
	int letra_cima[MAX_LETRAS];
	int achou=0;
	int numPalavrasPossiveis;
	
	//Chama a função que conta quantas palavras existem no arquivo.
	contaPalavras();
	//Sorteia a palavra.
	arquivo=fopen("./DADOS/dici.txt","r");
	
	do
	{
		do
		{
			palavraSorteada=(rand()*rand())%qtdadepalavras;
			for(i=0;i<palavraSorteada;i++)
			{
				fgets(novaPalavra,17,arquivo);
			}
			rewind(arquivo);
		}while(strlen(novaPalavra)<7);
		fclose(arquivo);
		minuscula(novaPalavra);
		novaPalavra[strlen(novaPalavra)-1]='\0';
		//Conta quantas palavras podem ser formadas com a palavra sorteada.	
		numPalavrasPossiveis=contaPalavrasPossiveis(novaPalavra);
	}while(numPalavrasPossiveis>100);
	
	
	//Aloca espaço na memória para todas as palavras q podem ser formadas.
	matriz =(char **)malloc(numPalavrasPossiveis * sizeof(char *));
	for (i=0;i<numPalavrasPossiveis;i++)
	{
		matriz[i]=(char *)malloc(MAX_LETRAS * sizeof(char));	
	}	
	//Chama a função que verifica quais palavras são possiveis e as coloca na matriz.
	verificaPalavrasPossiveis(novaPalavra,matriz);
	
	
	
	
	
	//Aloca espaço para o vetor que diz se a palavra já foi digitada ou não, e o zera.
	achadas=(int *)malloc(numPalavrasPossiveis*sizeof(int));	
	for (i=0;i<numPalavrasPossiveis;i++)
	{
		achadas[i]=0;
	}
	//Chama a função pro tempo rolar.
	tempo = TEMPO_DE_JOGO;
	LOCK_VARIABLE(tempo);
	LOCK_FUNCTION(incrementa_tempo);
	install_int(incrementa_tempo, 1000);
	//Embaralha a palavra sorteada.
	embaralhaPalavra(novaPalavra);
	//Calcula o início de onde vai começar a desenhar a palavra.
	inicio=(RESOLUCAO_HOR-(strlen(novaPalavra)*LARGURA_LETRA))/2;	
	while(1)
	{
		desenhaTela(novaPalavra,achadas,matriz,numPalavrasPossiveis);
		show_mouse(NULL);
		blit(screen,tela,65,460,0,0,50,20);
		show_mouse(screen);
		
		for (i=0;i<strlen(novaPalavra);i++)
		{
			palavra[i]='\0';
			letra_cima[i]=1;
		}
		i=0;
		while(i<strlen(novaPalavra) || !key[KEY_ENTER])
		{
			clear_keybuf();
			mouse_b=0;
			while(!(mouse_b & 1) && !keypressed())
			{
				clear(temp);
				blit(tela,temp,0,0,0,0,50,20);
				textprintf(temp,font,0,0,COR_DAS_LETRAS,"%2d:%1.2d", tempo / 60,tempo % 60);
				blit(temp,screen,0,0,65,460,50,20);
				if (tempo<=0)
				{
					if (proxima_fase)
					{
						game_over=0;
						return 0;
					}
					else
					{
						game_over=1;
						return 0;
					}
				}
			}
			if (mouse_b & 1)
			{
				j=0;
				while (j<strlen(novaPalavra))
				{
					if (mouse_x>inicio+LARGURA_LETRA*j && mouse_x<inicio+LARGURA_LETRA*(j+1) && mouse_y>LINHA_PALAVRA_ORIGINAL && mouse_y<LINHA_PALAVRA_ORIGINAL+ALTURA_LETRA && letra_cima[j]==1)
					{
						letra=novaPalavra[j];
						break;
					}
					j++;
				}		
				if (mouse_x>322 && mouse_x<477 && mouse_y>543 && mouse_y<583)	
				{
					if (i<3)
					{
						avisoMaior3();
					}
					else
					{
						palavra[i]='\0';
						break;
					}
				}
				else if (mouse_x>635 && mouse_x<790 && mouse_y>355 && mouse_y<395)
				{
					if (i)
					{
						i--;
						for (j=0;j<strlen(novaPalavra);j++)
						{
							if (palavra[i]==novaPalavra[j] && letra_cima[j]==0)
							{	
								desenhaLetra(novaPalavra[j],inicio+j*LARGURA_LETRA,LINHA_PALAVRA_ORIGINAL);
								letra_cima[j]=1;
								break;
							}	
						}		
						apagaLetra(inicio+i*LARGURA_LETRA,LINHA_PALAVRA_FORMADA);
						palavra[i]='\0';
					}
				}
				else if(mouse_x>635 && mouse_x<790 && mouse_y>455 && mouse_y<495)
				{
					embaralhaPalavra(novaPalavra);
					for (j=0;j<strlen(novaPalavra);j++)
					{
						palavra[j]='\0';
						letra_cima[j]=1;
					}
					i=0;
					desenhaTela(novaPalavra,achadas,matriz,numPalavrasPossiveis);
				}
				else if (mouse_x>635 && mouse_x<790 && mouse_y>536 && mouse_y<576)
				{
					if (proxima_fase)
					{
						game_over=0;
						return 0;
					}
					else
					{
						game_over=1;
						return 0;
					}
				}
				if (verificaLetra(letra,novaPalavra,palavra))
				{
					palavra[i]=letra;
					desenhaLetra(letra,inicio+i*LARGURA_LETRA,LINHA_PALAVRA_FORMADA);
					apagaLetra(inicio+(j)*LARGURA_LETRA,LINHA_PALAVRA_ORIGINAL);
					letra_cima[j]=0;
					i++;
				}
				letra='\0';
				
				
			}
			else
			{
				letra=readkey() & 0xFF;
				minuscula(&letra);
				if (verificaLetra(letra,novaPalavra,palavra))
				{
					palavra[i]=letra;
					desenhaLetra(letra,inicio+i*LARGURA_LETRA,LINHA_PALAVRA_FORMADA);
					for (j=0;j<strlen(novaPalavra);j++)
					{
						if (novaPalavra[j]==palavra[i] && letra_cima[j]==1)
						{	
							apagaLetra(inicio+j*LARGURA_LETRA,LINHA_PALAVRA_ORIGINAL);
							letra_cima[j]=0;
							break;
						}
					}
					i++;
				}
				else if (key[KEY_ENTER])
				{
					if (i<3)
					{
						avisoMaior3();
					}
					else
					{
						palavra[i]='\0';
						break;
					}
				}
				else if (key[KEY_BACKSPACE])
				{
					if (i)
					{
						i--;
						for (j=0;j<strlen(novaPalavra);j++)
						{
							if (palavra[i]==novaPalavra[j] && letra_cima[j]==0)
							{	
								desenhaLetra(novaPalavra[j],inicio+j*LARGURA_LETRA,LINHA_PALAVRA_ORIGINAL);
								letra_cima[j]=1;
							break;
						}	
					}		
						apagaLetra(inicio+i*LARGURA_LETRA,LINHA_PALAVRA_FORMADA);
						palavra[i]='\0';
					}
				}
				else if (key[KEY_SPACE])
				{
					embaralhaPalavra(novaPalavra);
					for (j=0;j<strlen(novaPalavra);j++)
					{
						palavra[j]='\0';
						letra_cima[j]=1;
					}
					i=0;
					desenhaTela(novaPalavra,achadas,matriz,numPalavrasPossiveis);
				}
				else if (key[KEY_F2])
				{
					if (proxima_fase)
					{
						game_over=0;
						return 0;
					}
					else
					{
						game_over=1;
						return 0;
					}
				}
			}
		}
		achou=0;				
		for (i=0;i<numPalavrasPossiveis;i++)
		{
			if (compara(palavra,matriz[i]) && achadas[i]==0)
			{
				achou=1;
				achadas[i]=1;
				if(strlen(novaPalavra)==strlen(palavra))
				{
					proxima_fase=1;
				}
				break;
			}
			else if (compara(palavra,matriz[i]) && achadas[i]==1)
			{
				achou=2;
				break;						
			}
		}
		if (achou==1) 
		{
			pontos+=strlen(palavra)*10;
			achei();
		}
		else if (achou==2)
		{
			acheiDenovo();
		}
		else
		{
			naoAchei();
		}
		i=0;
		while(i<numPalavrasPossiveis)
		{
			if (achadas[i]==0)
			{
				break;
			}
			i++;
		}
		if (i==numPalavrasPossiveis)
		{
			tempo=0;
		}
		
					
	}
	return 0;
}

void desenhaTela(char *palavra,int *achadas,char **matriz,int n)
{
	int cont;
	int inicio;
	BITMAP *temp;
	BITMAP *letra;
	BITMAP *fundo;
	BITMAP *proxima;
	PALLETE letra_pal,fundo_pal,pal;
	show_mouse(NULL);
	temp=create_bitmap(RESOLUCAO_HOR,RESOLUCAO_VER);
	fundo=load_bmp("./GRAFICOS/fundo.bmp",fundo_pal);
	blit(fundo,temp,0,0,0,0,RESOLUCAO_HOR,RESOLUCAO_VER);
	inicio=(RESOLUCAO_HOR-(strlen(palavra)*LARGURA_LETRA))/2;
	for (cont=0;cont<strlen(palavra);cont++)
	{
		switch (palavra[cont])
		{
			case 'a':
				letra=load_bmp("./GRAFICOS/a.bmp",letra_pal);
				set_pallete(letra_pal);
				break;
			case 'b':
				letra=load_bmp("./GRAFICOS/b.bmp",letra_pal);
				set_pallete(letra_pal);
				break;
			case 'c':
				letra=load_bmp("./GRAFICOS/c.bmp",letra_pal);
				set_pallete(letra_pal);
				break;
			case 'd':
				letra=load_bmp("./GRAFICOS/d.bmp",letra_pal);
				set_pallete(letra_pal);
				break;
			case 'e':
				letra=load_bmp("./GRAFICOS/e.bmp",letra_pal);
				set_pallete(letra_pal);
				break;
			case 'f':
				letra=load_bmp("./GRAFICOS/f.bmp",letra_pal);
				set_pallete(letra_pal);
				break;
			case 'g':
				letra=load_bmp("./GRAFICOS/g.bmp",letra_pal);
				set_pallete(letra_pal);
				break;
			case 'h':
				letra=load_bmp("./GRAFICOS/h.bmp",letra_pal);
				set_pallete(letra_pal);
				break;
			case 'i':
				letra=load_bmp("./GRAFICOS/i.bmp",letra_pal);
				set_pallete(letra_pal);
				break;
			case 'j':
				letra=load_bmp("./GRAFICOS/j.bmp",letra_pal);
				set_pallete(letra_pal);
				break;
			case 'l':
				letra=load_bmp("./GRAFICOS/l.bmp",letra_pal);
				set_pallete(letra_pal);
				break;
			case 'm':
				letra=load_bmp("./GRAFICOS/m.bmp",letra_pal);
				set_pallete(letra_pal);
				break;
			case 'n':
				letra=load_bmp("./GRAFICOS/n.bmp",letra_pal);
				set_pallete(letra_pal);
				break;
			case 'o':
				letra=load_bmp("./GRAFICOS/o.bmp",letra_pal);
				set_pallete(letra_pal);
				break;
			case 'p':
				letra=load_bmp("./GRAFICOS/p.bmp",letra_pal);
				set_pallete(letra_pal);
				break;
			case 'q':
				letra=load_bmp("./GRAFICOS/q.bmp",letra_pal);
				set_pallete(letra_pal);
				break;
			case 'r':
				letra=load_bmp("./GRAFICOS/r.bmp",letra_pal);
				set_pallete(letra_pal);
				break;
			case 's':
				letra=load_bmp("./GRAFICOS/s.bmp",letra_pal);
				set_pallete(letra_pal);
				break;
			case 't':
				letra=load_bmp("./GRAFICOS/t.bmp",letra_pal);
				set_pallete(letra_pal);
				break;
			case 'u':
				letra=load_bmp("./GRAFICOS/u.bmp",letra_pal);
				set_pallete(letra_pal);
				break;
			case 'v':
				letra=load_bmp("./GRAFICOS/v.bmp",letra_pal);
				set_pallete(letra_pal);
				break;
			case 'x':
				letra=load_bmp("./GRAFICOS/x.bmp",letra_pal);
				set_pallete(letra_pal);
				break;
			case 'z':
				letra=load_bmp("./GRAFICOS/z.bmp",letra_pal);
				set_pallete(letra_pal);
				break;
		}
		draw_sprite(temp,letra,inicio+cont*LARGURA_LETRA,LINHA_PALAVRA_ORIGINAL);
	}
	letra=load_bmp("./GRAFICOS/apagar.bmp",letra_pal);
	for (cont=0;cont<strlen(palavra);cont++)
	{
		draw_sprite(temp,letra,inicio+cont*LARGURA_LETRA,LINHA_PALAVRA_FORMADA);
	}
	
	text_mode(-1);
	textprintf(temp,font,90,400,COR_DAS_LETRAS,"%d",pontos);
	//IMPRIME QUADRO
	int i,j;
	int linha=0;
	int coluna=0;
	for (i=0;i<n;i++)
	{
		maiuscula(matriz[i]);
		for (j=0;j<strlen(matriz[i]);j++)
		{
			if (achadas[i])
			{
				textprintf(temp,font,180+10*j+100*coluna,35+10*linha,COR_DAS_LETRAS,"%c",matriz[i][j]);
			}
			else
			{
				textprintf(temp,font,180+10*j+100*coluna,35+10*linha,COR_DAS_LETRAS,"_ ");
			}
		}
		minuscula(matriz[i]);
		linha++;
		if (linha==20)
		{
			linha=0;
			coluna++;
		}
		
	}
	//FIM DE EMPRIMIR QUADRO
	if (proxima_fase)
	{
		proxima=load_bitmap("./GRAFICOS/proximafase.bmp",pal);
		draw_sprite(temp,proxima,635,536);
	}
	blit(temp,screen,0,0,0,0,RESOLUCAO_HOR,RESOLUCAO_VER);
	destroy_bitmap(temp);
	destroy_bitmap(letra);
	show_mouse(screen);
}

void letraA(int x,int y)
{
	BITMAP *a;PALLETE aa;a=load_bmp("./GRAFICOS/a.bmp",aa);set_pallete(aa);
	draw_sprite(screen,a,x,y);
	destroy_bitmap(a);
}
void letraB(int x,int y)
{
	BITMAP *b;PALLETE bb;b=load_bmp("./GRAFICOS/b.bmp",bb);set_pallete(bb);
	draw_sprite(screen,b,x,y);
	destroy_bitmap(b);
}
void letraC(int x,int y)
{
	BITMAP *c;PALLETE cc;c=load_bmp("./GRAFICOS/c.bmp",cc);set_pallete(cc);
	draw_sprite(screen,c,x,y);
	destroy_bitmap(c);
}
void letraD(int x,int y)
{
	BITMAP *d;PALLETE dd;d=load_bmp("./GRAFICOS/d.bmp",dd);set_pallete(dd);
	draw_sprite(screen,d,x,y);
	destroy_bitmap(d);
}
void letraE(int x,int y)
{
	BITMAP *e;PALLETE ee;e=load_bmp("./GRAFICOS/e.bmp",ee);set_pallete(ee);
	draw_sprite(screen,e,x,y);
	destroy_bitmap(e);
}
void letraF(int x,int y)
{
	BITMAP *f;PALLETE ff;f=load_bmp("./GRAFICOS/f.bmp",ff);set_pallete(ff);
	draw_sprite(screen,f,x,y);
	destroy_bitmap(f);
}
void letraG(int x,int y)
{
	BITMAP *g;PALLETE gg;g=load_bmp("./GRAFICOS/g.bmp",gg);set_pallete(gg);
	draw_sprite(screen,g,x,y);
	destroy_bitmap(g);
}
void letraH(int x,int y)
{
	BITMAP *h;PALLETE hh;h=load_bmp("./GRAFICOS/h.bmp",hh);set_pallete(hh);
	draw_sprite(screen,h,x,y);
	destroy_bitmap(h);
}
void letraI(int x,int y)
{
	BITMAP *i;PALLETE ii;i=load_bmp("./GRAFICOS/i.bmp",ii);set_pallete(ii);
	draw_sprite(screen,i,x,y);
	destroy_bitmap(i);
}
void letraJ(int x,int y)
{
	BITMAP *j;PALLETE jj;j=load_bmp("./GRAFICOS/j.bmp",jj);set_pallete(jj);
	draw_sprite(screen,j,x,y);
	destroy_bitmap(j);
}
void letraL(int x,int y)
{
	BITMAP *l;PALLETE ll;l=load_bmp("./GRAFICOS/l.bmp",ll);set_pallete(ll);
	draw_sprite(screen,l,x,y);
	destroy_bitmap(l);
}
void letraM(int x,int y)
{
	BITMAP *m;PALLETE mm;m=load_bmp("./GRAFICOS/m.bmp",mm);set_pallete(mm);
	draw_sprite(screen,m,x,y);
	destroy_bitmap(m);
}
void letraN(int x,int y)
{
	BITMAP *n;PALLETE nn;n=load_bmp("./GRAFICOS/n.bmp",nn);set_pallete(nn);
	draw_sprite(screen,n,x,y);
	destroy_bitmap(n);
}
void letraO(int x,int y)
{
	BITMAP *o;PALLETE oo;o=load_bmp("./GRAFICOS/o.bmp",oo);set_pallete(oo);
	draw_sprite(screen,o,x,y);
	destroy_bitmap(o);
}
void letraP(int x,int y)
{
	BITMAP *p;PALLETE pp;p=load_bmp("./GRAFICOS/p.bmp",pp);set_pallete(pp);
	draw_sprite(screen,p,x,y);
	destroy_bitmap(p);
}
void letraQ(int x,int y)
{
	BITMAP *q;PALLETE qq;q=load_bmp("./GRAFICOS/q.bmp",qq);set_pallete(qq);
	draw_sprite(screen,q,x,y);
	destroy_bitmap(q);
}
void letraR(int x,int y)
{
	BITMAP *r;PALLETE rr;r=load_bmp("./GRAFICOS/r.bmp",rr);set_pallete(rr);
	draw_sprite(screen,r,x,y);
	destroy_bitmap(r);
}
void letraS(int x,int y)
{
	BITMAP *s;PALLETE ss;s=load_bmp("./GRAFICOS/s.bmp",ss);set_pallete(ss);
	draw_sprite(screen,s,x,y);
	destroy_bitmap(s);
}
void letraT(int x,int y)
{
	BITMAP *t;PALLETE tt;t=load_bmp("./GRAFICOS/t.bmp",tt);set_pallete(tt);
	draw_sprite(screen,t,x,y);
	destroy_bitmap(t);
}
void letraU(int x,int y)
{
	BITMAP *u;PALLETE uu;u=load_bmp("./GRAFICOS/u.bmp",uu);set_pallete(uu);
	draw_sprite(screen,u,x,y);
	destroy_bitmap(u);
}
void letraV(int x,int y)
{
	BITMAP *v;PALLETE vv;v=load_bmp("./GRAFICOS/v.bmp",vv);set_pallete(vv);
	draw_sprite(screen,v,x,y);
	destroy_bitmap(v);
}
void letraX(int w,int y)
{
	BITMAP *x;PALLETE xx;x=load_bmp("./GRAFICOS/x.bmp",xx);set_pallete(xx);
	draw_sprite(screen,x,w,y);
	destroy_bitmap(x);
}
void letraZ(int x,int y)
{
	BITMAP *z;PALLETE zz;z=load_bmp("./GRAFICOS/z.bmp",zz);set_pallete(zz);
	draw_sprite(screen,z,x,y);
	destroy_bitmap(z);
}
void apagaLetra(int x,int y)
{
	show_mouse(NULL);
	BITMAP *quadro;
	PALLETE pal;
	quadro=load_bmp("./GRAFICOS/apagar.bmp",pal);
	draw_sprite(screen,quadro,x,y);
	show_mouse(screen);
	destroy_bitmap(quadro);
}
void desenhaLetra(char l,int x,int y)
{
	show_mouse(NULL);
	switch(l)
	{
		case 'a':
			letraA(x,y);
			break;
		case 'b':
			letraB(x,y);
			break;
		case 'c':
			letraC(x,y);
			break;
		case 'd':
			letraD(x,y);
			break;
		case 'e':
			letraE(x,y);
			break;
		case 'f':
			letraF(x,y);
			break;
		case 'g':
			letraG(x,y);
			break;
		case 'h':
			letraH(x,y);
			break;
		case 'i':
			letraI(x,y);
			break;
		case 'j':
			letraJ(x,y);
			break;
		case 'l':
			letraL(x,y);
			break;
		case 'm':
			letraM(x,y);
			break;
		case 'n':
			letraN(x,y);
			break;
		case 'o':
			letraO(x,y);
			break;
		case 'p':
			letraP(x,y);
			break;
		case 'q':
			letraQ(x,y);
			break;
		case 'r':
			letraR(x,y);
			break;
		case 's':
			letraS(x,y);
			break;
		case 't':
			letraT(x,y);
			break;
		case 'u':
			letraU(x,y);
			break;
		case 'v':
			letraV(x,y);
			break;
		case 'x':
			letraX(x,y);
			break;
		case 'z':
			letraZ(x,y);
			break;
	}
	show_mouse(screen);
}
void incrementa_tempo (void)
{
	tempo--;
}
END_OF_FUNCTION(incrementa_tempo);

void contaPalavras(void)
{
	FILE *p;
	char temp[MAX_LETRAS];
	p=fopen("./DADOS/dici.txt","r");
	if (!p)
	{
		perror("fopen");
	}
	qtdadepalavras=0;
	while(!feof(p))
	{
		fgets(temp,MAX_LETRAS,p);
		qtdadepalavras++;
	}	
	fclose(p);
}

int contaPalavrasPossiveis(char *v)
{	
	FILE *p;
	int cont;
	int i,j,pode;
	int letras[26],le[26];
	char secundaria[MAX_LETRAS];
	cont=0;
	for(i=0;i<26;i++)
	{
		letras[i]=0;
	}	
	for (i=0;i<strlen(v);i++)
	{
		letras[v[i]-'a']++;
	}
	p=fopen("./DADOS/dici.txt","r");
	for (j=0;j<qtdadepalavras;j++)
	{
		fgets(secundaria,MAX_LETRAS,p);
		secundaria[strlen(secundaria)-1]='\0';
		pode=1;
		for(i=0;i<26;i++)
		{
			le[i]=0;
		}	
		for (i=0;i<strlen(secundaria);i++)
		{
			le[secundaria[i]-'a']++;
		}
		for (i=0;i<26;i++)
		{
			if (le[i]>letras[i])
			{
				pode=0;
				break;
			}
		}
		if (pode)
		{
			cont++;
		}
	}
	fclose(p);
	return cont;	
}
void verificaPalavrasPossiveis(char *v,char **matriz)
{
	FILE *p;
	int cont;
	int i,j,pode;
	int letras[26],le[26];
	char secundaria[MAX_LETRAS];
	cont=0;
	for(i=0;i<26;i++)
	{
		letras[i]=0;
	}	
	for (i=0;i<strlen(v);i++)
	{
		letras[v[i]-'a']++;
	}
	p=fopen("./DADOS/dici.txt","r");
	for (j=0;j<qtdadepalavras;j++)
	{
		fgets(secundaria,MAX_LETRAS,p);
		secundaria[strlen(secundaria)-1]='\0';
		pode=1;
		for(i=0;i<26;i++)
		{
			le[i]=0;
		}	
		for (i=0;i<strlen(secundaria);i++)
		{
			le[secundaria[i]-'a']++;
		}
		for (i=0;i<26;i++)
		{
			if (le[i]>letras[i])
			{
				pode=0;
				break;
			}
		}
		if (pode)
		{
			for (i=0;i<strlen(secundaria);i++)
			{
				matriz[cont][i]=secundaria[i];			
			}
			matriz[cont][i]='\0';
			cont++;
		}
	}
	//Ordena as palavras possiveis por ordem de tamanho.
	for (i=0;i<cont;i++)
	{
		for (j=0;j<cont-1-i;j++)
		{
			if (strlen(matriz[j]) > strlen(matriz[j+1]) )
			{
			strcpy(secundaria,matriz[j+1]);
			strcpy(matriz[j+1],matriz[j]);
			strcpy(matriz[j],secundaria);
			}	
		}	
	}
	fclose(p);
}

