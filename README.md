# FSE-2021_2_projeto_01
Trabalho 1 da disciplina de Fundamentos de Sistemas Embarcados (2021/2)

## Objetivo 
Este trabalho tem por objetivo a implementação de um sistema (que simula) o controle de um forno para soldagem de placas de circuito impresso (PCBs).

## Links importantes
Para acessar o repositório template para o trabalho apresentado, clique [aqui](https://gitlab.com/fse_fga/trabalhos-2021_2/trabalho-1-2021-2).

## Observações
### Tipos de controle:
* Terminal
    - Nessa opção o usuário irá definir a Temperatura Referencial (TR), além de, definir os valores de Controle Proporcional (Kp), Controle Integral (Ki) e o Controle Derivativo (Kd), caso queira. Caso essas opções não sejam passadas, o programa definirá valores padrões, 
    sendo eles: Kp: 20, Ki: 0.1 e Kd: 100.
* Potenciomêtro
    - Nessa opção o usuário irá coletar informações diretas da placa.
* Potenciomêtro
    - Incompleto. Está lendo os valores do .csv mas não fazendo as requisições pelo tempo requisitado.

## Gráficos 
### Potenciômetro
Gráfico gerado seguindo os seguintes valores: 
* Temperatura ReferenciaL (TR): Obtida por meio do potenciômetro. 
* Temperatura Externa (TE): Obtida por meio do sensor bme280(I2C).
* Temperatura Interna (TI): Obtida por meio do sensor ds18b20(1-WIRE).

<img src="./images/potenciometro.png" width="450px" alt="potenciometro">

## Como executar?
> Clone este repositório:

    git clone https://github.com/victordsantoss/FSE-2021_2_projeto_01
> Acesse a pasta raiz do repositório:

    cd FSE-2021_2_projeto_01
> Na pasta raiz do repositório crie uma pasta chamada bin:

    mkdir bin
> Execute na pasta raiz do repositório:
    
    make
> Rode o processo de execução

    ./bin/bin
