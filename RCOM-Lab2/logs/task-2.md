# Task 2 - Implement two bridges in a switch

## 1.
Conectar e configurar o Tux32 e registar o seu endereço de IP e MAC:

```bash
$ ifconfig eth0 up
$ ifconfig eth0 172.16.31.1
```

## 2.
Criar duas bridged no switch (`bridge30` e `bridge31`).

```bash
> /interface bridge add name=bridge30
> /interface bridge add name=bridge31
```

## 3.
Remover as portas onde o Tux33, Tux34 e Tux32 estão conectados a partir da bridge default e adicioná-las às bridges bridge30 e bridge31.

```bash
# Remover
> /interface bridge port remove [find interface=ether1]
> /interface bridge port remove [find interface=ether2]
> /interface bridge port remove [find interface=ether3]

# Adicionar
> /interface bridge port add bridge=bridge30 interface=ether1
> /interface bridge port add bridge=bridge30 interface=ether2
> /interface bridge port add bridge=bridge31 interface=ether3
```

## 4.
Através do Wireshark, começar a capturar Tux33.eth0 .

## 5.
A partir do Tux33, dar ping ao tux34 e, de seguida, dar ping ao Tux32.

```bash
$ ping 172.16.30.254   # correspondente ao Tux34
$ ping 172.16.31.1     # correspondente ao Tux32
```

## 6.
Parar a captura e guardar os logs.

## 7.1.
Começar novas capturas do Tux32.eth0, Tux33.eth0 e Tux34.eth0 .

## 8.1.
No Tux33, dar ping broadcast durante alguns segundos.

```bash
$ ping -b 172.16.30.255
```

## 9.1.
Observar os resultados obtidos, parar a captura e guardar os logs.

## 7.2.
Começar novas capturas do Tux32.eth0, Tux33.eth0 e Tux34.eth0 .

## 8.2.
No Tux32, dar ping broadcast durante alguns segundos.

```bash
$ ping -b 172.16.31.255
```

## 9.2.
Observar os resultados obtidos, parar a captura e guardar os logs.