# Sistema de Controle Supervisório - Arduino

## Introdução
Este projeto implementa um sistema interativo em Arduino com um menu exibido em um **display LCD 16x2 (via I2C)** e navegação controlada por **quatro botões (push buttons)**. O sistema permite o controle e a leitura de diferentes dispositivos conectados ao Arduino, oferecendo funcionalidades como leitura de sensores, controle de relés e LEDs, e envio de mensagens para o LCD.

## Componentes Utilizados
- Arduino Uno (ou compatível)
- Display LCD 16x2 com interface I2C
- 4 Push Buttons:
  - **Subir**: Navegar para cima no menu
  - **Descer**: Navegar para baixo no menu
  - **Selecionar**: Escolher uma opção do menu
  - **Voltar**: Retornar ao menu principal
- Sensor de umidade e temperatura (DHT11 ou DHT22)
- Potenciômetro
- Relé (para controle de dispositivos externos)
- LEDs (vermelho e amarelo)

## Funcionalidades
O sistema possui um menu interativo com as seguintes opções:

### **1. Mostrar Valor do Potenciômetro**
- Exibe no LCD o valor lido de um potenciômetro conectado a uma entrada analógica.
- A leitura é continuamente atualizada enquanto esta opção está ativa.
- Permite retornar ao menu principal pressionando o botão "Voltar".

---

### **2. Mostrar Temperatura e Umidade**
- Utiliza um sensor DHT para exibir os valores de temperatura e umidade no LCD.
- Se o sensor falhar, uma mensagem de erro será exibida.
- Permite retornar ao menu principal pressionando o botão "Voltar".

---

### **3. Controle do Relé**
- Alterna entre **ligar** e **desligar** o relé.
- O estado atual do relé é exibido no LCD.
- Retorna ao menu principal pressionando o botão "Voltar".

---

### **4. Enviar Mensagem para o LCD**
- Abre o monitor serial para que o usuário digite uma mensagem personalizada.
- A mensagem enviada pelo monitor serial é exibida no LCD.
- Permite retornar ao menu principal pressionando o botão "Voltar".

---

### **5. Controle de LEDs**
#### **5.1. LED Vermelho**
- Alterna o estado (ligado/desligado) de um LED vermelho conectado ao Arduino.
- Mostra o estado atual no LCD.
- Retorna ao menu principal pressionando o botão "Voltar".

#### **5.2. LED Amarelo**
- Alterna o estado (ligado/desligado) de um LED amarelo.
- Mostra o estado atual no LCD.
- Retorna ao menu principal pressionando o botão "Voltar".

---

### **6. Controle da Backlight do LCD**
- Abre um submenu que permite ligar ou desligar a luz de fundo do LCD.
- Mostra o estado atual no LCD.
- Permite retornar ao menu principal pressionando o botão "Voltar".

---

## Navegação pelo Menu
- **Subir (Botão 1)**: Move a seta do menu para cima.
- **Descer (Botão 2)**: Move a seta do menu para baixo.
- **Selecionar (Botão 3)**: Acessa a funcionalidade correspondente à opção selecionada.
- **Voltar (Botão 4)**: Retorna ao menu principal de qualquer submenu.

### Exibição do Menu
O menu é exibido de forma interativa com uma seta (`>`), indicando a opção selecionada. A navegação é feita verticalmente, e o menu é atualizado dinamicamente.

---

## Estrutura do Código
O código segue a seguinte organização:

1. **Setup**:
   - Configura as entradas e saídas.
   - Inicializa o display LCD e o monitor serial.
   - Exibe o menu inicial.

2. **Loop**:
   - Lida com a navegação no menu.
   - Chama as funções correspondentes às opções selecionadas.

3. **Funções Auxiliares**:
   - **`showMenu()`**: Exibe o menu principal no LCD.
   - **`executeOption(int menuIndex)`**: Executa a funcionalidade correspondente à opção selecionada.
   - **Submenus específicos**: Implementam cada funcionalidade descrita.

---

## Conclusão
Este projeto demonstra como criar um sistema de controle interativo usando Arduino, combinando leitura de sensores, controle de dispositivos e navegação por menus. A interface com o LCD e os botões proporciona uma experiência amigável para o usuário, enquanto a modularidade do código permite fácil expansão e personalização.

Sinta-se à vontade para adaptar o código às suas necessidades ou integrar novas funcionalidades! 🚀
