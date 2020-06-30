LoRaOxi Oxímetro IoT Over LoRaWan

Este projeto tem dois objetivos:

Primeiro é desenvolver um oxímetro de ponta de dedo que seja integrado a tecnologia LoRaWan para ser utilizado em hospitais públicos no Brasil. 

Este dispositivo tem por objetivo permitir que médicos e outras equipes médicas rastreiem mais facilmente a saturação de oxigênio do paciente quase em tempo real.

Em segundo é fornecer a um guia para que qualquer pessoa que queira construir um para ser doado a hospitais públicos.
O projeto é baseado no oxímetro de pulso de ponta de dedo Yonker YK-80, que está prontamente disponível online. 

O oxímetro funciona medindo a absorção de diferentes comprimentos de onda da luz ao passar pela ponta do dedo; a hemoglobina oxigenada exibe maior absorção do que a hemoglobina normal e, portanto, as medições fornecem uma visão dos níveis de hemoglobina oxigenada e, finalmente, do sp02 do paciente (pressão parcial de oxigênio). 

Para a maioria dos indivíduos, o sp02 medido deve ser superior a 90%; qualquer coisa menor pode indicar que a pessoa não está recebendo oxigênio adequado. 

No ambiente hospitalar, isso é importante ao monitorar pacientes em ventilação e / ou outros pacientes críticos que possam necessitar de ventilação.

Normalmente, todos os hospitais públicos terão equipamentos que podem medir o sp02 desses pacientes; no entanto, atualmente cabe à equipe de enfermagem observar mudanças significativas e entrar em contato com o médico de plantão. 

Devido à natureza da saúde pública, isso não pode ou nem sempre é feito em tempo hábil e coloca em risco o paciente. Embora exista equipamento com técnicas automatizadas de geração de relatórios em vários back-ends de redes sem fio, geralmente é caro e requer um nível de sofisticação tecnológica e infraestrutura que não é viável na maioria dos hospitais públicos  do Brasil.

Existe uma clara necessidade que essa tecnologia seja econômica para operar e possa operar sem depender do acesso à Internet. 
Este projeto nasceu para atender a essa necessidade.














OximetroFrontend
Frontend desenvolvido com o framework Angular (versão 9.1.4) para o projeto do oxímetro IoT com a PUC

Servidor de Desenvolvimento
Execute ng serve para iniciar um servidor de desenvolvimento e a aplicação estará disponível em http://localhost:4200/. A aplicação irá reiniciar automaticamente caso o comando seja executado como sudo e após forem identificadas modificações salvas no código fonte.
Build
Execute ng build para fazer o build do projeto. Os artefatos gerados serão armazenados no diretório dist/. Use a flag --prod para fazer o build do código para produção.
O código de produção foi disponibilizado no como arquivos estáticos no repositório do Backend

https://github.com/inmetro-lainf/oximetro-frontend
Backend do serviço de Oximetria
Serviço desenvolvido com o microframework Flask com o proposito de fornecer um frontend e conectar com um banco de dados InfluxDB.
Projeto desenvolvido em parceria entre PUC-RJ e Inmetro
Container Docker
A execução do Web Service é realizada através de um contêiner Docker e a configuração foi realizada através de Dockerfile e docker-compose. O arquivo config.cfg com os parâmetros de configuração para o ambiente de prodrução (Endereço IP do banco de dados, senhas e etc.) deve ser disponibilizado durante a configuração do contêiner. Os arquivos docker-compose.yml e Dockerfile são descritos a seguir:
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

# Cria diretório de trabalho
RUN mkdir -p /srv/app/
WORKDIR /srv/app

# Clona repositorio do github
RUN git clone https://github.com/inmetro-lainf/oximetro-backend
WORKDIR oximetro-backend/

# Instala dependências
RUN pip install -r requirements.txt

# Copia arquivo com configurações de produção
RUN mkdir backend/production_instance/
COPY  config.cfg backend/production_instance/

EXPOSE 8080

CMD [ "python", "run.py", "production" ]
É preciso estar atento que o contêiner é executado com network_mode: bridge e nesse caso o contêiner não conseguirá acessar outros contêiners na mesma rede através da resolução de nome, apenas realizará o acesso através do endereço IP na subrede Docker.
https://github.com/inmetro-lainf/oximetro-backend



