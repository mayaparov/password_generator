#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QCheckBox>
#include <QClipboard>
#include <QComboBox>
#include <QDesktopServices>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSlider>
#include <QUrl>
#include <QVBoxLayout>
#include <QWidget>
#include <iostream>
#include <openssl/rand.h>
#include <random>
#include <string>

/**
 * @brief Главное окно приложения "Генератор паролей/ключей".
 *
 * Класс `MainWindow` реализует графический интерфейс приложения
 * для генерации паролей и ключей.
 */
class MainWindow : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор класса `MainWindow`.
     *
     * Инициализирует элементы интерфейса и связывает сигналы и слоты.
     * @param parent Указатель на родительский виджет (по умолчанию nullptr).
     */
    MainWindow(QWidget *parent = nullptr);

private slots:
    /**
     * @brief Обработчик события изменения выбора типа (Ключ/Пароль).
     *
     * Управляет состоянием флажков в зависимости от выбранного типа.
     * @param index Индекс выбранного элемента в выпадающем списке.
     */
    void onTypeComboBoxChanged(int index);

    /**
     * @brief Обработчик события изменения значения ползунка.
     *
     * Обновляет текстовое поле ввода длины пароля в соответствии с
     * текущим значением ползунка.
     * @param value Новое значение ползунка.
     */
    void onLengthSliderChanged(int value);

    /**
     * @brief Обработчик события изменения текста в поле ввода длины.
     *
     * Обновляет значение ползунка в соответствии с введенным значением длины,
     * если оно находится в допустимом диапазоне.
     * @param text Новое значение текста в поле ввода.
     */
    void onLengthLineEditChanged(const QString &text);

    /**
     * @brief Обработчик события нажатия на кнопку "Сгенерировать".
     *
     * Проверяет, соответствует ли длина пароля ограничениям,
     * генерирует пароль с заданными параметрами и отображает результат.
     */
    void onGenerateButtonClicked();

    /**
     * @brief Обработчик события нажатия на кнопку "Скопировать".
     *
     * Копирует сгенерированный пароль в буфер обмена и закрывает окно результата.
     */
    void onCopyButtonClicked();

private:
    /**
     * @brief Функция генерации пароля/ключа.
     *
     * Генерирует случайный пароль или ключ заданной длины,
     * используя заданные параметры: использование прописных букв,
     * цифр и символов.
     *
     * @param length Длина генерируемого пароля/ключа.
     * @param useUppercase Флаг, указывающий, следует ли использовать прописные буквы.
     * @param useNumbers Флаг, указывающий, следует ли использовать цифры.
     * @param useSymbols Флаг, указывающий, следует ли использовать символы.
     *
     * @return Сгенерированный пароль/ключ в виде строки std::string.
     */
    std::string generatePassword(int length, bool useUppercase, bool useNumbers, bool useSymbols);

    // Элементы интерфейса
    QComboBox *typeComboBox;
    QSlider *lengthSlider;
    QLineEdit *lengthLineEdit;
    QPushButton *generateButton;
    QWidget *resultWindow;
    QLabel *resultLabel;
    QPushButton *copyButton;
    QCheckBox *uppercaseCheckbox;
    QCheckBox *lowercaseCheckbox;
    QCheckBox *numbersCheckbox;
    QCheckBox *symbolsCheckbox;
};

#endif // MAINWINDOW_H
