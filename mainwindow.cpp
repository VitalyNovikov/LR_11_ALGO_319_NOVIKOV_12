#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>      // Для показу повідомлень (помилки, попередження)
#include <QFileDialog>      // Для виклику діалогових вікон відкриття/збереження файлів
#include <QFile>            // Для роботи з файлами
#include <QTextStream>      // Для читання/запису тексту з/до файлу

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // Ініціалізація інтерфейсу
    ui->setupUi(this);

    // Підключення пунктів меню до відповідних слотів
    connect(ui->actionLoad_from_file, &QAction::triggered,
            this, &MainWindow::on_actionLoad_from_file_triggered);

    connect(ui->actionSave_to_file, &QAction::triggered,
            this, &MainWindow::on_actionSave_to_file_triggered);
}

MainWindow::~MainWindow()
{
    delete ui; // Очищення виділеної пам’яті
}

// Слот для обробки натискання кнопки "Обчислити"
void MainWindow::on_pushButton_calc_clicked()
{
    float d, L;     // Змінні для діаметра та довжини кола
    bool ok;        // Прапорець успішного перетворення введення

    // Зчитування значення з поля вводу le_d
    d = ui->le_d->text().toFloat(&ok);

    if (ok && d > 0) {
        // Обчислення довжини кола (L = π * d), де π ≈ 3.14
        L = 3.14 * d;

        // Виведення результату у мітку l_result
        ui->l_result->setText(QString::number(L));
    }
    else {
        // Повідомлення про помилку у введенні
        QMessageBox::warning(this, "Помилка", "Некоректне значення діаметра!");

        // Очистити поле введення і результат
        ui->le_d->clear();
        ui->l_result->setText("0");

        // Повернути фокус на поле введення
        ui->le_d->setFocus();
    }
}

// Слот для завантаження даних з файлу
void MainWindow::on_actionLoad_from_file_triggered()
{
    // Відкриття діалогового вікна для вибору файлу
    QString filePath = QFileDialog::getOpenFileName(this, "Відкрити файл",
                                                    QDir::currentPath(),
                                                    "Текстові файли (*.txt);;Усі файли (*)");

    if (!filePath.isEmpty()) {
        QFile file(filePath);

        // Перевірка чи відкрився файл
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(this, "Помилка", "Не вдалося відкрити файл для читання.");
            return;
        }

        // Створення текстового потоку для читання
        QTextStream in(&file);
        QString sd;
        in >> sd;

        // Запис зчитаного значення у поле введення
        ui->le_d->setText(sd);

        file.close(); // Закрити файл
    } else {
        // Якщо файл не було вибрано
        QMessageBox::information(this, "Увага", "Файл не вибрано.");
    }
}

// Слот для збереження результату у файл
void MainWindow::on_actionSave_to_file_triggered()
{
    // Відкриття діалогового вікна для збереження
    QString filePath = QFileDialog::getSaveFileName(this, "Зберегти файл",
                                                    QDir::currentPath(),
                                                    "Текстові файли (*.txt);;Усі файли (*)");

    if (!filePath.isEmpty()) {
        QFile file(filePath);

        // Перевірка чи відкрився файл для запису
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::critical(this, "Помилка",
                                  "Не вдалося відкрити файл:\n" + file.errorString());
            return;
        }

        // Створення текстового потоку для запису
        QTextStream out(&file);

        // Запис результату у файл
        out << "Діаметр: " << ui->le_d->text() << "\n";
        out << "Довжина кола: " << ui->l_result->text() << "\n";

        file.close(); // Закрити файл
    } else {
        // Якщо користувач скасував вибір файлу
        QMessageBox::information(this, "Увага", "Файл не вибрано.");
    }
}
