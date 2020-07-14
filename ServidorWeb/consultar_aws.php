<?php
    require "validador_acesso.php";   //protege o acesso a pagina para usuarios logados
    require 'aws/aws-autoloader.php';      //SDK para requisições aws
    use Aws\DynamoDb\Exception\DynamoDbException;
    date_default_timezone_set('America/Sao_Paulo');      //configura a timezone

    $sdk = new Aws\Sdk([
        'region'   => 'us-east-2',                    //configura a localizacao da database dynamodb
        'version'  => 'latest',
        'credentials' => [
        	'key'    => '',                      //chave de identificacao da database        
        	'secret' => '',  //senha de acesso a database
        ],
    ]);

    $dynamodb = $sdk->createDynamoDb();                          //cria uma sessao de requisicao na aws
    $tableName = 'IoTData';
    
    if(isset($_GET['begin_time']) && isset($_GET['end_time'])){      //exige preenchimento dos inputs de pesquisa
        $time1 = (string) strtotime($_GET['begin_time']);            //selecao de itens pesquisados UnixTIMESTAMP
        $time2 = (string) strtotime($_GET['end_time']);
        $tamanho = (int) $time2 - (int) $time1;
        if ($tamanho < 3600){
            $indice_busca = 1;  // para até uma hora a cada 30 segundos
        }
        else if ($tamanho >= 3600 && $tamanho < 7200){
            $indice_busca = 2;   // cada 5 minutos
        }
        else if ($tamanho >= 7200 && $tamanho <14400 ){
            $indice_busca = 3;  //cada 15 minutos
        }
        else {
            $indice_busca = 4;  //cada duas horas
        }
        
        $eav = array                      //array de query de pesquisa
        (
            ':device_id' => array('S' => '0'),
            ':time1' => array('S' => $time1),
            ':time2' => array('S' => $time2),
            ':selecao' => array('S' => (string) $indice_busca)  
        );

        $params = [                       //array de requisição conforme modelo da aws
        	'TableName' => $tableName,
            'KeyConditionExpression' => 
            '#dvcid = :device_id and #tmstamp between :time1 and :time2',
            'FilterExpression' => 'contains(#indx, :selecao)',
            'ExpressionAttributeNames'=> [ '#dvcid' => 'id', '#tmstamp' => 'timestamp', '#indx' => 'indice'],
            'ExpressionAttributeValues'=> $eav
        ];
        if ($tamanho > 2592000){
            ?><script type="text/javascript">
            alert("O período máximo para consulta é de um mês.");
            </script><?php
        } else 
        {
            try {
                $result = $dynamodb->query($params);    //executa a requisição
            } catch (DynamoDbException $e) {
                echo "Unable to query:\n";
                echo $e->getMessage() . "\n";
            }
        }
    }
    function grafico_data($result){       //recebe o resultado da requisição e organiza o conteudo em um novo array
      	$buff=array();
        foreach ($result['Items'] as $idx => $item) {
            $buff[]=array(
                (int) $item['timestamp']['S'], 
                (float) $item['Tensao']['S'], 
                (float) $item['Corrente']['S'],
                (float) $item['Pot_Re']['S'],
                (float) $item['Pot_Ap']['S'],
                (float) $item['Pot_At']['S'],
                (float) $item['Freq']['S'],
                (float) $item['FP']['S']           
            );
        }
        return $buff;
    }
    if(isset($result)){
        $data = grafico_data($result);      
    }
?>

<html>
  <head>
    <meta charset="utf-8" />
    <title>Power Monitor App</title> <!--titulo da janela-->
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css" integrity="sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm" crossorigin="anonymous">
    <style>
        .card-home {
            padding: 30px 0 0 0;
            width: 100%;
            margin: 0 auto;
        }
    </style>
    
    <!-- O codigo abaixo é uma api do googlecharts para geração de gráficos-->    
    <script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script> 
    <script type="text/javascript">
        google.charts.load('current', {'packages':['corechart']});
        google.charts.setOnLoadCallback(drawChart);        

        function drawChart() {              //funcao de montagem do googlechart
            var data = new google.visualization.DataTable();
               data.addColumn('number', 'TimeStamp');   //adicao das colunas
                data.addColumn('number', 'Tensão');
                data.addColumn('number', 'Corrente');
                data.addColumn('number', 'Pot. Reativa');
                data.addColumn('number', 'Pot. Aparente');
                data.addColumn('number', 'Pot. Ativa');
                data.addColumn('number', 'Frequência');
                data.addColumn('number', 'Fator de Potência');
                data.addRows(<?php echo json_encode($data); ?>);  //adicao das linhas de dados presentes do array $data

            var options = {   //opcoes de criação do grafico
                title: 'Power Measurements',
                hAxis: {
                    title: 'Linha do tempo',
                    },
                curveType: 'function',
                legend: { position: 'right' },
                logScale: true   //escala logaritmica permite a melhor visualização de todos os valores no mesmo grafico
            


            };

            colunas_ativas = [{
                type: 'datetime',
                label: data.getColumnLabel(0),
                calc: function (dt, row) {
                    var timestamps = dt.getValue(row, 0) * 1000; // convert to milliseconds
                    return new Date(timestamps);
                }
            }, 1, 2, 3, 4, 5, 6, 7];  //variavel que armazena as colunas que estao visiveis no grafico
            
            var chart = new google.visualization.LineChart(document.getElementById('curve_chart'));  //seleção do garfico do tipo linechart
            chart.draw(data, options);  //chamada de criação do grafico
            view = new google.visualization.DataView(data);
            view.setColumns(colunas_ativas);
            
            chart.draw(view, options);
            
            function altera_colunas(inds) {  //inverte o estado de exibicao da coluna informada em 'inds' entre true e false
                var auxiliar = colunas_ativas.indexOf(inds);
                if (auxiliar === -1){
                    colunas_ativas.push(inds)
                } else {
                    colunas_ativas.splice(auxiliar, 1);
                }
                view.setColumns(colunas_ativas);
                chart.draw(view, options);
            }

            var oculta_tensao = document.getElementById("btTensao").onclick = function() {  //funcao executada durante o clique no botao bttensao
                view = new google.visualization.DataView(data);
                altera_colunas(1);
            }
            
            var oculta_corrente = document.getElementById("btCorrente").onclick = function() {
                view = new google.visualization.DataView(data);
                altera_colunas(2);
            }
            
            var oculta_pot_reativa = document.getElementById("btPotRe").onclick = function() {
                view = new google.visualization.DataView(data);
                altera_colunas(3);
            }
            
            var oculta_pot_aparente = document.getElementById("btPotAp").onclick = function() {
                view = new google.visualization.DataView(data);
                altera_colunas(4);
            }
            
            var oculta_pot_ativa = document.getElementById("btPotAt").onclick = function() {
                view = new google.visualization.DataView(data);
                altera_colunas(5);
            }
            
            var oculta_freq = document.getElementById("btFreq").onclick = function() {
                view = new google.visualization.DataView(data);
                altera_colunas(6);
            }
            
            var oculta_fp = document.getElementById("btFP").onclick = function() {
                view = new google.visualization.DataView(data);
                altera_colunas(7);
            }

            var resetar = document.getElementById("btReset");  //habilita a visualizacao de todas as colunas
            resetar.onclick = function() {
                view = new google.visualization.DataView(data);
                colunas_ativas = [0, 1, 2, 3, 4, 5, 6, 7];
                view.setColumns(colunas_ativas);
            }
        }
    </script>
  </head>


<style> 
.bt {
border:1px solid #25692A;
border-radius:4px;
display:inline-block;
cursor:pointer;
font-family:Verdana;
font-weight:bold;
font-size:13px;
padding:6px 10px;
text-decoration:none;
}
</style>

  <body>
    <nav class="navbar navbar-dark bg-dark">
      <a class="navbar-brand" href="#">
        <img src="logo.png" width="30" height="30" class="d-inline-block align-top" alt="">
        Power Monitor App  <!--Titulo ao lado da imagem -->
      </a>
      <ul class="navbar-nav">
        <li class="nav-item">
          <a class="nav-link" href="logoff.php">SAIR</a>  <!--Executa logoff da página -->
      </ul>
    </nav>


    <div>
    <center>
              <br><h5><br>Selecione a data para visualização: </h5>
    </center>
    <div class="col-15 d-flex justify-content-center">
        <table border="0" width="500">
            <tr>
                <th>
                    Inicio:
                </th>
                <th>
                    Fim:
                </th>
            </tr>
            <tr>
                <form>
                    <td>
                        <input type="datetime-local" id="begin_time" name="begin_time" 
                            <?php 
                                if(isset($_GET['begin_time'])){
                                    echo('value='.$_GET['begin_time'] ); //no caso de atualizar a pagina preenche automaticamente as input com os valores anteriores
                                }
                                else {
                                    $date1hourago = strtotime('-1 hour');
                                    echo('value='.date('Y-m-d\TH:j',$date1hourago)); //senao atualiza com a data atual
                                }
                            ?>                            
                        >
                    </td>
                    <td>
                            <input type="datetime-local" id="end_time" name="end_time" 
                            <?php 
                                if(isset($_GET['end_time'])){
                                    echo('value='.$_GET['end_time'] ); 
                                }
                                else {
                                echo('value='.date('Y-m-d\TH:j')); //senao atualiza com a data atual
                                }
                            ?>                            
                            >
                        </td>
                        <td>
                            <input type="submit" value="Pesquisar"></button>
                        </td>
                     </form>
                </tr>
            </table>
        </div>                               
    </div>

    <div style="float: right; width: 18%"  align="center">
        <br>
        <h6>Valores médios no período selecionado:<br></h6>
        <?php
            if (isset($data[0])){      //calcula as médias dos valores pesquisados 
                $somatorio = [0, 0, 0, 0, 0, 0, 0, 0];
                $unidade=array('', 'V', 'A', 'VAr', 'VA', 'W', 'Hz', '');
                $descricao=array('', 'Tensão: ', 'Corrente: ', 'Pot. Reativa: ', 'Pot. Aparente: ', 'Pot. Ativa: ', 'Frequência: ', 'Fator de potência: ');
                $media = array();
                $qtd_data = count($data);
                $qtd_variaveis = count($data[0]);
                foreach($data as $indx => $registro){
                    foreach ($registro as $indx2 => $parametro) {
                    $somatorio[$indx2] += $parametro;                       
                    }
                }
                foreach($somatorio as $indx3 => $valor){
                    if ($indx3 == 0) continue;
                    $media[$indx3] = $valor / ($qtd_data);
                    echo "$descricao[$indx3] $media[$indx3] $unidade[$indx3]<br>";
                }
            }
            ?>
        <h6><br><br>Estimativa conforme informações deste período: <br></h6>
        <?php
        if (isset($data[0])){      //calcula as médias dos valores pesquisados
            $consumo_mensal = ($media[5]*720)/1000;
            echo ("Consumo mensal: ".number_format($consumo_mensal,0)." kWh") ;
        }
        ?>
        
    </div>
          
  
    <div class="container">
            <div class="row">
                <div class="card-home">
                  <div class="card">
                    <div class="card-header">
                      Gráfico
                    </div>
                    <div class="card-body">
                        <div id="curve_chart" style="width: 1000px; height: 500px"></div>
                    </div>
                  </div>
                  <div class="col-15 d-flex justify-content-center">
                     <h6>Ativar / Desativar Visualização: &nbsp; </h6>
                     <button class="bt" type="button" id="btReset">Mostrar todos</button>
                     <button class="bt" type="button" id="btTensao">Tensao</button>
                     <button class="bt" type="button" id="btCorrente">Corrente</button>
                     <button class="bt" type="button" id="btPotRe">Pot. Reativa</button>
                     <button class="bt" type="button" id="btPotAp">Pot. Aparente</button>
                     <button class="bt" type="button" id="btPotAt">Pot. Ativa</button>
                     <button class="bt" type="button" id="btFreq">Frequência</button>
                     <button class="bt" type="button" id="btFP">Fator de Potencia</button>
                  </div>
                </div>
            </div>
    </div>
    <div style="clear: both"></div>
  </body>
</html>
