#include <dialog.h>
#include <ui_dialog.h>
#include <QFileDialog>
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    clearImage = new QPixmap;
    fileName = new QString;

    lay = new ZLayout;
    connect(this,SIGNAL(resEvent()),lay,SLOT(updateWidgets()));
    lay->initMainWidgets(this,ui->widget);
    lay->addItem(ui->drawingLabel);
    lay->addItem(ui->browseButton);
    lay->addItem(ui->cancelButton);
    lay->addItem(ui->pictureLabel);
    lay->addItem(ui->okButton);
    lay->addItem(ui->groupBox);
    lay->addItem(ui->x);
    lay->addItem(ui->y);
    lay->addItem(ui->width);
    lay->addItem(ui->height);
    lay->addItem(ui->label);
    lay->addItem(ui->label_2);
    lay->addItem(ui->label_3);
    lay->addItem(ui->label_4);
    lay->addItem(ui->status);
}

Dialog::~Dialog()
{
    delete clearImage;
    delete fileName;
    delete ui;
}

void Dialog::on_browseButton_clicked()
{
    QString fileDialog = QFileDialog::getOpenFileName(this,
                                                      tr("Resim seçin"),
                                                      "/home/",
                                                      tr("Resim Dosyası (*.png *.jpg *.bmp)"));
    qApp->processEvents();
    qApp->processEvents();
    qApp->processEvents();
    qApp->processEvents();
    qApp->processEvents();
    qApp->processEvents();
    qApp->processEvents();

    if (fileDialog.isEmpty() || fileDialog.isNull()){
        return;
    }

    *fileName = fileDialog;
    if (!clearImage->load(*fileName)){
        qWarning("Resim dosyası okunamıyor!");
        return;
    }
    ui->pictureLabel->setPixmap(clearImage->scaled(ui->pictureLabel->size()));
    ui->drawingLabel->setRect(QRectF(0,0,0,0));
    ui->okButton->setEnabled(true);
    ui->drawingLabel->setOk(true);
}

// Çıkış yapaıldığında diyalog result'unu olumsuza kitleyip diyalogu close ile gizler
void Dialog::on_cancelButton_clicked()
{
    this->done(QDialog::Rejected);
}

// Resize olduğunda çizilmiş olan rect'i siler ve aynı zamanda ZLayout'u tetikliyor.
// Aynı zamanda bununla birlikte labellerde temizleniyor(width, x, y.. labelleri)
void Dialog::resizeEvent(QResizeEvent *)
{
    ui->drawingLabel->setRect(QRectF(0,0,0,0));
    emit resEvent();
}

// Her paintEventte labelleri güncelliyoruz. Bunu Hep painteventte yapıyoruz
// Çünkü drawingLabel(üzerine plaka boxu çizdiğimiz label) hep paint evette çiziliyor.
void Dialog::paintEvent(QPaintEvent*)
{
    ui->x->setText(QString::number(ui->drawingLabel->getRect().x()));
    ui->y->setText(QString::number(ui->drawingLabel->getRect().y()));
    ui->width->setText(QString::number(ui->drawingLabel->getRect().width()));
    ui->height->setText(QString::number(ui->drawingLabel->getRect().height()));
}

// Status labele kullanıcıyı uyarması için verilen mesajı basar
void Dialog::setStatus(const QString& stat) const
{
    ui->status->setText(stat);
}

// Çizilmiş olan plaka çerçevesinin rectini döndürüyor
const QRectF Dialog::getRect() const
{
    return ui->drawingLabel->getRect();
}

// pictureLabel'in rect'ini yani tüm resmin boyutunu geri döner
const QRect Dialog::getRectPic() const
{
    return ui->pictureLabel->rect();
}

// Diyalog result'unu eğer plaka bölgesi seçilmişse olumluya
// yok eğer seçilmemişse olumsuza kitleyip diyalogu close ile gizler
void Dialog::on_okButton_clicked()
{
    if (ui->drawingLabel->getRect().isNull())
        this->done(QDialog::Rejected);
    else
        this->done(QDialog::Accepted);
}
