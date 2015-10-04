// TCP/IP Client Uygulaması

// Eklenen Kütüphaneler
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

//Hata Mesajları için metod
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main()
{

    //Değişkenler ve yapılar tanımlanıyor
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    //Haberleşme için tampon veri dizisi
    char buffer[256];

    //Konsol uygulaması için eklenen kısımlar çıkartıldı
    /*if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }*/
    //portno = atoi(argv[2]);


    portno=5000;    //Port 5000 olarak tanımlandı
    sockfd = socket(AF_INET, SOCK_STREAM, 0); //Soket açılıyor
    if (sockfd < 0)                 //sockfd return değeri -1 ise hata olduğu anlamına geliyor
        error("Soket Açılamadı");
    //server = gethostbyname(argv[1]);
    server=gethostbyname("192.168.1.177");   //local ip ataması
    if (server == NULL) {                   // server bağlantısı kontrol ediliyor
        fprintf(stderr,"Sunucu bulunamadı\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET; //server giriş ailesi AF_INET olarak tanımlanıyor
    bcopy((char *)server->h_addr,   //server bilgileri yapılara kopyalanıyor
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);  //Host to Network Short tür dönüşümü port numarası yapılıyor

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)   //bağlantı yapılıyor
        error("Bağlantı hatası");    //connect() metodunun geri dönüş verisi -1 ise hata veriyor
    while(1){ //devamlı veri gönderimi için sonsuz döngü
    printf("Gonderilecek veriyi giriniz: ");
    bzero(buffer,256);      // Tampon veriler temizleniyor
    fgets(buffer,255,stdin);  //girdiler tampona yazılıyor
    n = write(sockfd,buffer,strlen(buffer));   //tampon veri sokete gönderiliyor
    if (n < 0)
         error("Sokete yazım yapılamıyor");
    bzero(buffer,256);     //tampon tekrar temizleniyor
    n = read(sockfd,buffer,255);    //sokete gelen veri tampona yazılıyor
    }
    if (n < 0)                      //read() metodunun geri dönüş değeri -1 ise hata veriyor
         error("Soket okuma hatası");
    printf("%s\n",buffer);          //Okunan tampon veri ekrana yazdırılıyor
    close(sockfd);                  //soket kapatılıyor

    return 0;
}
