# Caracterização de voice spoofing para fins de verificação de locutores com base na transformada wavelet e na análise paraconsistente de características
# Characterization of voice spoofing: Announcers verification based on the wavelet transform and paraconsistent analysis of characteristics [English Version](#english-version)

# Universidade do estado de São Paulo - UNESP

## Resumo
Voice spoofing é uma estratégia genérica utilizada para burlar sistemas de autenticação biométrica baseados em identificação por voz. Dentre as diversas possibilidades específicas, os ataques do tipo playback speech são os que têm recebido considerável atenção da comunidade científica. Assim, por meio da decomposição dos sinais de voz com wavelets e posterior análise das respectivas sub-bandas espectrais BARK e MEL, este trabalho dedica-se a determinar qual a melhor combinação BARK/MEL-wavelet para que se obtenha uma separação máxima entre duas classes: Locuções genuínas e falseadas. Após a apuração da melhor combinação de descritores, realizada por meio da Análise Paraconsistente, os vetores de características oriundos dos sinais de voz são submetidos a ensaios de classificação, variando-se o tamanho do conjunto de treinamento e testes. Utilizando as distâncias Euclidiana e Manhattan, além de Máquinas de Vetores de Suporte (SVM), a acurácia máxima obtida foi de 99,7561% para uma base com 820 sinais, a qual considera-se como um resultado promissor frente àqueles existentes na literatura.

## Manual do código
Para realização deste trabalho foram produzidos variados textos (este incluso) e códigos, é recomendada a consulta dos códigos fontes utilizados nos procedimentos como complemento que, se espera, melhore o entendimento dos assuntos discutidos.
	
Uma atenção especial deve ser dada ao diretório localizado em [src/lib](./src/lib "Bibliotecas") que contém todas as bibliotecas criadas e referenciadas para resolver os problemas surgidos e também ao [src/experiments](./src/experiments "Procedimentos") que contém os procedimentos realizados. 

Em [soundSamples](./soundSamples "Base de dados com áudios") se encontra a base de dados com os áudios originais e tratados.
	
Alguns scripts foram desenvolvidos para facilitar o tratamento dos arquivos de áudio, os mesmos se encontram em [src/scripts](./src/scripts "Scripts auxiliares").
		
Por fim a parte escrita pode ser consultada em [documentation](.documentation "Parte escrita").
		
Todos esses materiais estão sob licença de código aberto (GPLv3) e são livres para uso não comercial.

# English Version

# São Paulo State university - UNESP

## Abstract
Voice spoofing is a generic strategy designed to circumvent biometric systems based on voice identification. Among a diversity of specific possibilities, playback speech attacks have received considerable attention from the scientific community. Thus, based on speech signals decomposition with wavelets for subsequent BARK and MEL scales spectral analysis, this dissertation aims at determining the best filters and scales to optimally separate between two classes: Genuine and spoofed speech. Once the best combination of descriptors is obtained, based on Paraconsistent Engineering, the feature vectors are subjected to classification, varying the randomly chosen training and test sets in size. Euclidean and Manhattan distances, as well as Support Vector Machine (SVM), were used as classifiers, where the highest value of accuracy was 99.7561% for a dataset with 820 signals. This is a promising result, considering the state-of-the-art in the field. 

## Code manual
To carry out this work, various texts (including this one) and codes were produced, it is recommended to consult the source codes used in the procedures as a complement that, hopefully, improves the understanding of the issues discussed. 

Special attention should be paid to the directory located at [src/lib](./src/lib "Libraries") which contains all the libraries created and referenced to solve the problems that have arisen and also to [src/experiments](./src/experiments "Procedures") that contains the procedures performed. 

In [soundSamples](./soundSamples "Database with audios") you will find the database with the original and treated audios.

Some scripts have been developed to facilitate the handling of audio files, they can be found in [src/scripts](./src/scripts "Auxiliary scripts").

Finally, the written part can be consulted in [documentation](./documentation "Written part").

All of these materials are under open source license (GPLv3) and are free for non-commercial use.
