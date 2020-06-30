LoRaOxi Ox�metro IoT Over LoRaWan

Este projeto tem dois objetivos:

Primeiro � desenvolver um ox�metro de ponta de dedo que seja integrado a tecnologia LoRaWan para ser utilizado em hospitais p�blicos no Brasil. 

Este dispositivo tem por objetivo permitir que m�dicos e outras equipes m�dicas rastreiem mais facilmente a satura��o de oxig�nio do paciente quase em tempo real.

Em segundo � fornecer a um guia para que qualquer pessoa que queira construir um para ser doado a hospitais p�blicos.
O projeto � baseado no ox�metro de pulso de ponta de dedo Yonker YK-80, que est� prontamente dispon�vel online. 

O ox�metro funciona medindo a absor��o de diferentes comprimentos de onda da luz ao passar pela ponta do dedo; a hemoglobina oxigenada exibe maior absor��o do que a hemoglobina normal e, portanto, as medi��es fornecem uma vis�o dos n�veis de hemoglobina oxigenada e, finalmente, do sp02 do paciente (press�o parcial de oxig�nio). 

Para a maioria dos indiv�duos, o sp02 medido deve ser superior a 90%; qualquer coisa menor pode indicar que a pessoa n�o est� recebendo oxig�nio adequado. 

No ambiente hospitalar, isso � importante ao monitorar pacientes em ventila��o e / ou outros pacientes cr�ticos que possam necessitar de ventila��o.

Normalmente, todos os hospitais p�blicos ter�o equipamentos que podem medir o sp02 desses pacientes; no entanto, atualmente cabe � equipe de enfermagem observar mudan�as significativas e entrar em contato com o m�dico de plant�o. 

Devido � natureza da sa�de p�blica, isso n�o pode ou nem sempre � feito em tempo h�bil e coloca em risco o paciente. Embora exista equipamento com t�cnicas automatizadas de gera��o de relat�rios em v�rios back-ends de redes sem fio, geralmente � caro e requer um n�vel de sofistica��o tecnol�gica e infraestrutura que n�o � vi�vel na maioria dos hospitais p�blicos  do Brasil.

Existe uma clara necessidade que essa tecnologia seja econ�mica para operar e possa operar sem depender do acesso � Internet. 
Este projeto nasceu para atender a essa necessidade.














OximetroFrontend
Frontend desenvolvido com o framework Angular (vers�o 9.1.4) para o projeto do ox�metro IoT com a PUC

Servidor de Desenvolvimento
Execute ng serve para iniciar um servidor de desenvolvimento e a aplica��o estar� dispon�vel em http://localhost:4200/. A aplica��o ir� reiniciar automaticamente caso o comando seja executado como sudo e ap�s forem identificadas modifica��es salvas no c�digo fonte.
Build
Execute ng build para fazer o build do projeto. Os artefatos gerados ser�o armazenados no diret�rio dist/. Use a flag --prod para fazer o build do c�digo para produ��o.
O c�digo de produ��o foi disponibilizado no como arquivos est�ticos no reposit�rio do Backend

https://github.com/inmetro-lainf/oximetro-frontend
Backend do servi�o de Oximetria
Servi�o desenvolvido com o microframework Flask com o proposito de fornecer um frontend e conectar com um banco de dados InfluxDB.
Projeto desenvolvido em parceria entre PUC-RJ e Inmetro
Container Docker
A execu��o do Web Service � realizada atrav�s de um cont�iner Docker e a configura��o foi realizada atrav�s de Dockerfile e docker-compose. O arquivo config.cfg com os par�metros de configura��o para o ambiente de prodru��o (Endere�o IP do banco de dados, senhas e etc.) deve ser disponibilizado durante a configura��o do cont�iner. Os arquivos docker-compose.yml e Dockerfile s�o descritos a seguir:
docker-compose.yml
version: "3"
services:
  oximetro-backend:
    image: lainf-inmetro/oximentro-backend
    network_mode: bridge
    build:
      context: ./
    ports:
      - "80:8080"
    restart: unless-stopped
    container_name: oximetro-backend
DockerFile
FROM python:3-alpine3.12

# Atualiza timezone do container
RUN apk add --no-cache tzdata
ENV TZ=America/Sao_Paulo
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Instala Git
RUN apk add git

# Cria diret�rio de trabalho
RUN mkdir -p /srv/app/
WORKDIR /srv/app

# Clona repositorio do github
RUN git clone https://github.com/inmetro-lainf/oximetro-backend
WORKDIR oximetro-backend/

# Instala depend�ncias
RUN pip install -r requirements.txt

# Copia arquivo com configura��es de produ��o
RUN mkdir backend/production_instance/
COPY  config.cfg backend/production_instance/

EXPOSE 8080

CMD [ "python", "run.py", "production" ]
� preciso estar atento que o cont�iner � executado com network_mode: bridge e nesse caso o cont�iner n�o conseguir� acessar outros cont�iners na mesma rede atrav�s da resolu��o de nome, apenas realizar� o acesso atrav�s do endere�o IP na subrede Docker.
https://github.com/inmetro-lainf/oximetro-backend



