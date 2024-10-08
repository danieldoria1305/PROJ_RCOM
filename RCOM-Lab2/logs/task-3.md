# Task 3 - Configure a Router in Linux

## 1.
Transformar Tux34 (Linux) em um router.<br>
&nbsp;- Configurar Tux34.eth1 e adicioná-lo à bridge31.

```bash
# Configuração
$ ifconfig eth1 up
$ ifconfig eth1 172.16.31.253

# Adicionar a bridges
/interface bridge port remove [find interface=ether4]
/interface bridge port add bridge=bridge31 interface=ether4
```

&nbsp;- Ativar IP forwarding.

```bash
echo 1 > /proc/sys/net/ipv4/ip_forward
```

&nbsp;- Desativar ICMP echo-ignore.

```bash
echo 0 > /proc/sys/net/ipv4/icmp_echo_ignore_broadcasts
```

## 2.
Observar os endereços MAC e IP nos Tux34.eth0 e Tux34.eth1 .

## 3.
Reconfigurar Tux33 e Tux32 para que os mesmos se consigam alcançar. Para isto, usamos o Tux34 como ponto de ligação.

```bash
route add -net 172.16.30.0/24 gw 172.16.31.253   # correspondente ao Tux32
route add -net 172.16.31.0/24 gw 172.16.30.254   # correspondente ao Tux33
```

## 4.
Observar as rotas disponiveis nos 3 Tux's.
```bash
route -n
```

## 5.
Começar a captura do Tux33 através do Wireshark.

## 6.
A partir do Tux33, dar ping aos endereços `172.16.30.254`, `172.16.31.253`, `172.16.31.1` e verificar se existe conectividade.

## 7.
Parar a captura e guardar os logs.

## 8.
Começar a captura no Tux34, usando duas instâncias do Wireshark, um por cada interface (eth0 e eth1).

## 9.
Limpar as tabelas ARP nos 3 Tux's.
```bash
arp -d 172.16.31.253  # correspondente ao Tux32
arp -d 172.16.30.254  # correspondente ao Tux33
arp -d 172.16.30.1    # correspondente ao Tux34
arp -d 172.15.31.1    # correspondente ao Tux34
```

## 10.
A partir do Tux33, dar ping ao Tux32 durante alguns segundos.

## 11.
Parar a captura no Tux34 e guardar os logs.