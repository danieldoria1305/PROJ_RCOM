# Task 1 - Configure an IP Network

## 1.
Conectar Tux33 e Tux34 ao switch. Para isso, conectar o E0 de ambos a uma entrada qualquer do switch.

## 2.
Configurar Tux33 e Tux34 usando os comandos `ifconfig` e `route`.

```bash
$ ifconfig eth0 up
$ ifconfig eth0 172.16.30.1     # correspondente ao Tux33
$ ifconfig eth0 172.16.30.254   # correspondente ao Tux34
```

## 3.
Registar o IP e o MAC de cada interface, utilizando o comando `ipconfig` e consultado o campo `ether`.

## 4.
Usar o comando `ping` e verificar a conectividade entre ambos os computadores.

```bash
$ ping 172.16.30.254   # correspondente ao Tux33
$ ping 172.16.30.1     # correspondente ao Tux34
```

## 5.
Inspecionar as tabelas forwarding e ARP do Tux33.

```bash
$ arp -a
```

## 6.
Eliminar as entradas da tabela ARP no Tux33.

```bash
$ arp -d 172.16.30.254
```

## 7.
Inicializar o Wireshark em `Tux33.eth0` e começar a capturar pacotes.

## 8. 
Dentro to Tux33, dar ping ao Tux34 durante alguns segundos. Para isso usar o comando:

```bash
$ ping 172.16.30.254 -c 15
```

## 9.
Parar a captura dos pacotes no Wireshark, guardar os resultados e avaliá-los.