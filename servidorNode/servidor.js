//  Servidor WEB escutando a porta 10000. Mostra os valores
//  de temperatura e distancia enviados pelos modulos sensores 
var express = require('express');
var app     = express();
var bodyParser = require('body-parser');
app.use(bodyParser.json()); 
var porta   = 10000;

app.post('/posts', function(request, response) {
	var tipoDado =request.body
	switch (tipoDado.modulo)
	{
		case 'temperatura':
			console.log("Recebeu leitura de temperatura=",
        tipoDado.valor);
			break
		case 'distancia':
			console.log("Recebeu leitura de distancia=",tipoDado.valor);
			break
	}
  response.end();
});

app.listen(porta);
console.log("servidor rodando na porta "+porta);
