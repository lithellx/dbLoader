//   ___|    _ \    __ \    ____|   __ \       __ )   \ \   /      |      _ _|  __ __|   |   |   ____|
//  |       |   |   |   |   __|     |   |      __ \    \   /       |        |      |     |   |   __|
//  |       |   |   |   |   |       |   |      |   |      |        |        |      |     ___ |   |
// \____|  \___/   ____/   _____|  ____/      ____/      _|       _____|  ___|    _|    _|  _|  _____|


#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <stdio.h>

#include <QMessageBox>
#include <QUrl>
#include <QDesktopServices>
#include <QClipboard>
#include <QtSql>
#include <QtSql/QSqlDatabase>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

QString hwid = "definitelyarealhwid123";
// Bu bir "hwid" değildir. Sadece sunucu ile aradaki bağlantıyı sağlayacak bir şifre gibi düşünün.
// Siz kendi hwid'nizi oluşturan fonksiyonunuzdan çıkan string ile sunucunuzdaki hwid'i eşleştiriceksiniz.

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // UI setup'ı bittikten sonra kodlarmızı yazmaya başlıyoruz.

    // Veritabanı ile bağlantı kuruyoruz.
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL"); //Database'imizi tanımlayarak ekliyoruz..
    db.setHostName("localhost"); // Veritabanımızın barındığı adresi yazıyoruz.
    db.setUserName("root"); // Veritabanı girişindeki (PhpMyAdmin) kullanıcı adını yazıyoruz.
    db.setPassword(""); // Veritabanı girişindeki şifremizi yazıyoruz. Localhosttayım şifre koymadım.
    db.setDatabaseName("dbloader"); // Database'in altındaki yapının adını yazıyoruz. (Table değil, yapı adı.)

    // Versiyon kontrolü, güncelleme kontrolü, açık/kapalı durumu kontrolünü bu şekilde yapabilirsiniz. Konumuz bu değil bunu geçiyorum.
    // Şimdi loader'ımıza giriş yapmamız gerekiyor. Bunu sağlayalım.

    // Loader'a giriş :)

    ui->hwidText->setText(hwid); // Burada çektiğimiz hwid'i formdaki textbox'ımıza geçiriyoruz.

    if(db.open()) // Eğer veritabanıyla bağlantımız var ise,
    {
        QString lHwid = ui->hwidText->text(); // Kontrol ettireceğimiz hwid'i tanımladık.
        QString lUyelikDurumu; // Üyelik durumumuzu veritabanından buraya tanımlayacağız.
        QString lTest; // Örnek amaçlı olarak veritabanından test yazısını da buraya tanımlayacağız.

        QString ExecuteSQL = "SELECT lUyelikDurumu, lTest FROM data WHERE hwid = '" + lHwid + "'"; // SQL sorgumuzun stringini oluşturuyoruz.
        //Bu satırda kısaca "lUyelikDurumu" ve "lTest"i, xorui tablosunun içindeki "hwid" ile bizim hwidimizin eşleştiği sorgu sonucunu getiriyor.

        QSqlQuery qry; // Sql sorgumuzu oluşturuyoruz.

        qry.prepare(ExecuteSQL); // SQL sorgumuzun stringini sql sorgumuzla birleştiriyoruz.
        if( !qry.exec() ) // Eğer sorgu çalışmadıysa 
            qDebug() << qry.lastError();
          else // Eğer sorgu çalıştıysa
          {
        QSqlRecord rec = qry.record();
          int cols = rec.count();

          QString temp;
          for( int c=0; c<cols; c++ )
            temp += rec.fieldName(c) + ((c<cols-1)?"\t":"");

          while( qry.next() )
              {
                temp = "";
                  lUyelikDurumu = qry.value(0).toString(); // Veritabanındaki lUyelikDurumu'nu başarıyla çektikten sonra buradaki az önce tanımladığımız "lUyelikDurumu" stringine geçiriyoruz.
                  lTest = qry.value(1).toString(); // Aynı şekilde burdaki de "lTest" için geçerli.
              }
          }

        // Sonuç olarak verileri çekmiş oluyoruz.

        ui->pushButton_4->setText(lUyelikDurumu); // Ve burada da Üyelik durumumuzu formumuza geçiriyoruz.
        ui->pushButton_6->setText(lTest); // Aynı şekilde Test yazımızı da geçiriyoruz.
    }
}

// Hwid kopyalama butonunun görevi burası.
void MainWindow::on_pushButton_5_clicked()
{
    QClipboard *clip = QApplication::clipboard(); // Kopyalama Panomuzu tanımlıyoruz.
    QString input = ui->hwidText->text(); // Input = Hwid olarak tanımlıyoruz.
    clip->setText(input); // Panomuza giriş olarak Input'ı geçiriyoruz.
}

MainWindow::~MainWindow()
{
    delete ui;
}

// lithé
