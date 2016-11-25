To compile and use the programs is necesary:

* Have an equal or greater 4.8.4 g++ version - Because the program has been built in c++11.

The way to use the program is:

* Go to bin directory.
* Type "conctrl", which has some optional arguments:
    * [--ficheroconfiguracion=<rutaficherocfg>]
    * [--semaforo=<id>]
    * [--memoriacompartida=<id>]
* It will creat a prompt with "conctrl>" and it is possible to type the following commands:
    * ’listar’ IdProcCtrl
    * ’sumar’ IdProcCtrl NUMERO
    * ’restar’ IdProcCtrl NUMERO
    * ’suspender’ IdProcCtrl
    * ’restablecer’ IdProcCtrl
    * ’indefinir’ IdProcCtrl
    * ’definir’ IdProcCtrl NUMERO
    * ’terminar’ IdProcCtrl
* Suicide processes will be appearing in the prompt whit its respective information.