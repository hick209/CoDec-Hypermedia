#CONTEUDOS DESTE ARQUIVO
   
 * Introducao
 * Requerimentos
 * Instalacao
 * Execucao
 * Troubleshooting


##INTRODUCAO

Trabalho desenvolvido na disciplina SSC661 - Multimidia e Hipermidia - sob orientação do professor Rudinei.

Para a versao git do projeto, acesse o link:
https://github.com/hick209/CoDec-Hypermedia.git


 **Grupo:**
	- Larissa Cardoso Zimmermann 	7126879
	- Nivaldo Henrique Bondanca	7143909
	- Raphael Victor Ferreira	7143889


##REQUERIMENTOS

Este codigo precisa ser compilado em C++. O Makefile é providenciado junto aos arquivos do projeto.
Para roda-lo em Windows, recomenda-se o uso do g++ em um terminal Cygwin.


##INSTALACAO

Basta executar o comando make na pasta do projeto para gerar o executavel codecFuckingFodaCodec.exe .


##EXECUCAO

### Lista de comandos disponiveis
	Para uma descricao completa de todos os comandos disponiveis, execute no terminal:
	`./codecFuckingFodaCodec.exe --help`

### Compressao
	Um exemplo de chamada de compressao por diferencas + huffman para o arquivo teste.wav seria:
	`./codecFuckingFodaCodec.exe -dh teste.wav`

### Descompressao 
	O arquivo .fuf guarda as compressoes realizadas sobre o arquivo .wav na ordem em que foram realizadas. Logo, para a descompressao de qualquer arquivo .fuf, a chamada será a mesma. Um exemplo de chamada de descompressao para o arquivo teste.fuf seria:
	`./codecFuckingFodaCodec.exe --dec teste.fuf`


##TROUBLESHOOTING

### Segmentation Fault
	O programa tem a capacidade de escrever 2 tipos de arquivos diferentes: Um arquivo ".fuf" a partir da compressao de um arquivo ".wav" ou um arquivo ".wav" a partir da descompressao do arquivo ".fuf". Para evitar receber segmentation fault como erro, cheque se a solicitacao de compressao foi realizada a partir de um arquivo ".wav" e a solicitacao de descompressao foi realizada para um arquivo ".fuf".