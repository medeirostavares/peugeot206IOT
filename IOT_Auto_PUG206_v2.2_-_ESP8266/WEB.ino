void webServer() {
  //Verifica se algum cliente está tentando se conectar
  WiFiClient client = server.available();

  if (client) {
    Serial.println("Novo cliente conectou");
    //Fazemos a leitura da requisição
    String req = client.readStringUntil('\r');
    Serial.print("Requisição: ");
    Serial.println(req);

    //Escreve o html no buffer que serÃ¡ enviado para o cliente
    client.print(htmlPage());
    //Envia os dados do buffer para o cliente
    client.flush();

    //Comandos_______________________________________________________
    /*
     * Creio que essa forma ak está errada, acho que o esp acaba executando
     * duas verificações sobre o msm comando.
     */
    if (req.indexOf("ignId=startIgnition") != -1) {
      controlSystem("startIgnition");
    } else if (req.indexOf("ignId=stopIgnition") != -1) {
      controlSystem("stopIgnition");
    } else if (req.indexOf("engId=startEngine") != -1) {
      controlSystem("startEngine");
    } else if (req.indexOf("lightId=lightsOn") != -1) {
      controlSystem("lightsOn");
    } else if (req.indexOf("lightId=lightsOff") != -1) {
      controlSystem("lightsOff");
    } else if (req.indexOf("lightId=meiaLuzOn") != -1) {
      controlSystem("meiaLuzOn");
    } else if (req.indexOf("lightId=meiaLuzOff") != -1) {
      controlSystem("meiaLuzOff");
    } else if (req.indexOf("lightId=alterSetas") != -1) {
      controlSystem("alterSetas");
    } else if (req.indexOf("hornID=hornStart") != -1) {
      controlSystem("hornStart");
    } else if (req.indexOf("lockId=lockStart") != -1) {
      controlSystem("lockStart");
    }

    if (req.indexOf("system=STAND_BY") != -1) {
      controlSystem("STAND_BY");
    }
  }
}

//Paginas HTML____________________________________________________________________________________________________________________________________________________________

String htmlPage() {
  String htmlPage = "  <!DOCTYPE html>";
  htmlPage += "  <html lang='pt-BR'>";
  
  htmlPage += "  <head>";
  htmlPage += "    <meta charset='utf-8'>";
  htmlPage += "    <meta name='viewport' content='width=device-width, initial-scale=1'>";
  
  htmlPage += "    <link rel='stylesheet' href='https://stackpath.bootstrapcdn.com/bootstrap/4.1.3/css/bootstrap.min.css' integrity='sha384-MCw98/SFnGE8fJT3GXwEOngsV7Zt27NXFoaoApmYm81iuXoPkFOJwJ8ERdknLPMO' crossorigin='anonymous'>";
  
  htmlPage += "    <title>Peugeot 206 IOT</title>";
  htmlPage += "  </head>";
  htmlPage += "  <body>";
  htmlPage += "    <br>";
  htmlPage += "    <div class='container-fluid'>";
  htmlPage += "      <div class='jumbotron'>";
  htmlPage += "        <h1 class='display-4'>Painel de Comandos - Peugeot 206 IOT</h1>";
  htmlPage += "        <p class='lead'>Instituto Federal de Educação Ciencia e Tecnologia do Tocantins - campus Dianopolis</p>";
  htmlPage += "        <hr class='my-4'>";
  htmlPage += "        <p>Professor: Fabiano Medeiro Tavares</p>";
  htmlPage += "        <p>Aluno: Joaquim Flávio Almeida Quirino Gomes</p>";
  htmlPage += "      </div><!--card-body-->";
  htmlPage += "    </div><!--card text-center-->";
  
  htmlPage += "    <div class='container'>";
  htmlPage += "      <div class='card-columns'>";
  
  htmlPage += "        <div class='card text-center' style='width: 18rem; margin: 15px;'>";
  htmlPage += "         <div class='card-body'>";
  htmlPage += "            <h3 class='card-title'>Travas</h3>";
  htmlPage += "            <br>";
  htmlPage += "            <a href='/?lockId=lockStart' class='btn btn-light'>Liga Desliga</a>";
  htmlPage += "          </div><!--card-body-->";
  htmlPage += "        </div><!--card text-center-->";
  
  htmlPage += "        <div class='card text-center' style='width: 18rem; margin: 15px;'>";
  htmlPage += "          <div class='card-body'>";
  htmlPage += "            <h3 class='card-title'>Ignição</h3>";
  htmlPage += "            <br>";
  htmlPage += "            <a href='/?ignId=startIgnition' class='btn btn-light'>Liga</a>";
  htmlPage += "            <a href='/?ignId=stopIgnition' class='btn btn-light'>Desliga</a>";
  htmlPage += "          </div><!--card-body-->";
  htmlPage += "        </div><!--card text-center-->";
  
  htmlPage += "        <div class='card text-center' style='width: 18rem; margin: 15px;'>";
  htmlPage += "          <div class='card-body'>";
  htmlPage += "            <h3 class='card-title'>Partida</h3>";
  htmlPage += "            <br>";
  htmlPage += "            <a href='/?engId=startEngine' class='btn btn-light'>START</a>";
  htmlPage += "          </div><!--card-body-->";
  htmlPage += "        </div><!--card text-center-->";
  
  htmlPage += "        <div class='card text-center' style='width: 18rem; margin: 15px;'>";
  htmlPage += "          <div class='card-body'>";
  htmlPage += "            <h3 class='card-title'>Faróis</h3>";
  htmlPage += "            <br>";
  htmlPage += "            <a href='/?lightId=lightsOn' class='btn btn-light'>Liga</a>";
  htmlPage += "            <a href='/?lightId=lightsOff' class='btn btn-light'>Desliga</a>";
  htmlPage += "          </div><!--card-body-->";
  htmlPage += "        </div><!--card text-center-->";
  
  htmlPage += "        <div class='card text-center' style='width: 18rem; margin: 15px;'>";
  htmlPage += "          <div class='card-body'>";
  htmlPage += "            <h3 class='card-title'>Meia Luz</h3>";
  htmlPage += "            <br>";
  htmlPage += "            <a href='/?lightId=meiaLuzOn' class='btn btn-light'>Liga</a>";
  htmlPage += "            <a href='/?lightId=meiaLuzOff' class='btn btn-light'>Desliga</a>";
  htmlPage += "          </div><!--card-body-->";
  htmlPage += "        </div><!--card text-center-->";
  
  htmlPage += "        <div class='card text-center' style='width: 18rem; margin: 15px;'>";
  htmlPage += "          <div class='card-body'>";
  htmlPage += "            <h3 class='card-title'>Setas</h3>";
  htmlPage += "            <br>";
  htmlPage += "            <a href='/?lightId=alterSetas' class='btn btn-light'>Liga Desliga</a>";
  htmlPage += "          </div><!--card-body-->";
  htmlPage += "        </div><!--card text-center-->";
  
  htmlPage += "        <div class='card text-center' style='width: 18rem; margin: 15px;'>";
  htmlPage += "          <div class='card-body'>";
  htmlPage += "            <h3 class='card-title'>Buzina</h3>";
  htmlPage += "            <br>";
  htmlPage += "            <a href='/?hornId=hornStart' class='btn btn-light'>Tocar</a>";
  htmlPage += "          </div><!--card-body-->";
  htmlPage += "        </div><!--card text-center-->";
  
  htmlPage += "        <div class='card text-center border-warning' style='width: 18rem; margin: 15px;'>";
  htmlPage += "          <div class='card-body text-warning'>";
  htmlPage += "            <h3 class='card-title'>Standy-by</h3>";
  htmlPage += "            <br>";
  htmlPage += "            <a href='/?system=STAND_BY' class='btn btn-light'>sleep</a>";
  htmlPage += "          </div><!--card-body-->";
  htmlPage += "        </div>";
  
  htmlPage += "      </div><!-- card-deck -->";
  htmlPage += "    </div><!--container-fluid-->";
  htmlPage += "  </body>";
  htmlPage += "  </html>";

  return htmlPage;
}
