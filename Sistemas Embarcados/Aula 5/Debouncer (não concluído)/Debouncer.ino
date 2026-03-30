/* ************************************************************************

Colégio SATC
Curso Técnico em Mecatrônica
Disciplina: Sistemas Embarcados
Turma: 3191
Professor: Marcos Antônio Jeremias Coelho
Aluno: João Pedro Coral

Programa: Debouncer: limitar o tempo de execução do arduino

Data: 23/03/2026
Versão: 1.0

************************************************************************ */

const int botao = 2;        //cria a variavel botao e atribui ela na entrada 2
bool estadoBotao = HIGH;    // cria a variável do estado atual do botão e atribui como high (=1) 
                            //pega o valor dado do momento atual
bool leituraAntiga = HIGH;  // cria uma variável pra primeira leitura do botão para comparar com o atual
                            //espera a oscilação mecânica do botão parar para assim ler o valor estável definitivo

unsigned long tempo = 0;    //// cria uma variavel pra guardar o momento em que a mudança aconteceu

const int debounce = 50;    // cria uma constante pra definir um tempo de estabiliziação do debounce

const int led_verde = 13;

void setup() {
  pinMode(botao, INPUT_PULLUP);  // configura o botão como pullup
  pinMode(led_verde, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  int leitura = digitalRead(botao);  // cria uma variavel chamada leitura e dai atribui a ela o valor do botao (no caso é a variavel da leitura do valor do botao)

  if (leitura != leituraAntiga) {  // se a leitura (valor do botao) for difente do ultimo valor lido
    tempo = millis();              // guarda o mili segundo exato da mudança de estado do botao
  }

  if (millis() - tempo > debounce) {  // verifica se ja passou o tempo do debouce
    estadoBotao = leitura;            // se millis - tempo for maior que debounce confirma que a leitura é confiavel (no caso o valor lido)
                                      //Se do valor do tempo que passou desde a mudança de valor - tempo
                                      //o valor de "timer" e o resultado for maior que o tempo definido no "debouncetimer"
  }
    if (estadoBotao == LOW) {         // se o estado do botao for pressionado a 13 liga
      digitalWrite(led_verde, HIGH);  // senão não liga
    } else {
      digitalWrite(led_verde, LOW);
    }
  }
    leituraAntiga = leitura;  // a leitura antiga agora terá o valor atribuido da leitura desse ciclo de maquina
    // fica dentro do loop
} 
