#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Генератор паролей/ключей");

    // Выпадающий список для выбора типа
    typeComboBox = new QComboBox();
    typeComboBox->addItem("Ключ");
    typeComboBox->addItem("Пароль");
    QLabel *typeLabel = new QLabel("Тип:");
    QHBoxLayout *typeLayout = new QHBoxLayout();
    typeLayout->addWidget(typeLabel);
    typeLayout->addWidget(typeComboBox);

    // Ползунок для выбора длины
    lengthSlider = new QSlider(Qt::Horizontal);
    lengthSlider->setMinimum(8);
    lengthSlider->setMaximum(256);
    lengthSlider->setValue(16);
    QLabel *lengthLabel = new QLabel("Длина:");
    QHBoxLayout *lengthLayout = new QHBoxLayout();
    lengthLayout->addWidget(lengthLabel);
    lengthLayout->addWidget(lengthSlider);

    // Текстовое поле для ввода длины
    lengthLineEdit = new QLineEdit();
    lengthLineEdit->setText("16");
    lengthLineEdit->setValidator(new QIntValidator(lengthSlider->minimum(), lengthSlider->maximum(), lengthLineEdit));
    QHBoxLayout *lengthLineEditLayout = new QHBoxLayout();
    lengthLineEditLayout->addWidget(lengthLineEdit);

    // Кнопка "Сгенерировать"
    generateButton = new QPushButton("Сгенерировать");

    // Окно результата
    resultWindow = new QWidget();
    resultWindow->setWindowTitle("Результат");
    resultLabel = new QLabel();
    copyButton = new QPushButton("Скопировать");

    // Компоновка окна результата
    QVBoxLayout *resultLayout = new QVBoxLayout();
    resultLayout->addWidget(resultLabel);
    resultLayout->addWidget(copyButton);
    resultWindow->setLayout(resultLayout);

    // Флажки для выбора символов
    uppercaseCheckbox = new QCheckBox("Прописные буквы");
    lowercaseCheckbox = new QCheckBox("Строчные буквы");
    numbersCheckbox = new QCheckBox("Цифры");
    symbolsCheckbox = new QCheckBox("Символы");
    symbolsCheckbox->setChecked(true);
    lowercaseCheckbox->setDisabled(true);
    QHBoxLayout *CheckBoxLayout = new QHBoxLayout();
    CheckBoxLayout->addWidget(uppercaseCheckbox);
    CheckBoxLayout->addWidget(lowercaseCheckbox);
    CheckBoxLayout->addWidget(numbersCheckbox);
    CheckBoxLayout->addWidget(symbolsCheckbox);


    // Обработка выбора типа
    connect(typeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onTypeComboBoxChanged);

    // Обработка изменения длины слайдера
    connect(lengthSlider, &QSlider::valueChanged, this, &MainWindow::onLengthSliderChanged);

    // Обработка изменения текста в поле ввода длины
    connect(lengthLineEdit, &QLineEdit::textChanged, this, &MainWindow::onLengthLineEditChanged);

    // Обработка нажатия на кнопку "Сгенерировать"
    connect(generateButton, &QPushButton::clicked, this, &MainWindow::onGenerateButtonClicked);

    // Обработка нажатия на кнопку "Скопировать"
    connect(copyButton, &QPushButton::clicked, this, &MainWindow::onCopyButtonClicked);

    // Компоновка
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(typeLayout);
    mainLayout->addLayout(lengthLayout);
    mainLayout->addLayout(lengthLineEditLayout);
    mainLayout->addLayout(CheckBoxLayout);
    mainLayout->addWidget(generateButton);
    setLayout(mainLayout);
}

void MainWindow::onTypeComboBoxChanged(int index) {
    /**
     * @brief Обработчик события изменения выбора типа (Ключ/Пароль).
     *
     * Управляет состоянием флажков в зависимости от выбранного типа.
     * @param index Индекс выбранного элемента в выпадающем списке.
     */
    if (index == 0) { // Ключ
        uppercaseCheckbox->setDisabled(false);
        lowercaseCheckbox->setDisabled(true);
        uppercaseCheckbox->setChecked(false);
        lowercaseCheckbox->setChecked(true);
    } else if (index == 1) { // Пароль
        uppercaseCheckbox->setDisabled(true);
        lowercaseCheckbox->setDisabled(false);
        uppercaseCheckbox->setChecked(true);
        lowercaseCheckbox->setChecked(false);
    }
}

void MainWindow::onLengthSliderChanged(int value) {
    /**
     * @brief Обработчик события изменения значения ползунка.
     *
     * Обновляет текстовое поле ввода длины пароля в соответствии с
     * текущим значением ползунка.
     * @param value Новое значение ползунка.
     */
    lengthLineEdit->setText(QString::number(value));
}

void MainWindow::onLengthLineEditChanged(const QString &text) {
    /**
     * @brief Обработчик события изменения текста в поле ввода длины.
     *
     * Обновляет значение ползунка в соответствии с введенным значением длины,
     * если оно находится в допустимом диапазоне.
     * @param text Новое значение текста в поле ввода.
     */
    bool ok;
    int value = text.toInt(&ok);
    if (ok) {
        if (value < lengthSlider->minimum() || value > lengthSlider->maximum()) {
            QMessageBox::warning(this, "Ошибка", QString("Длина пароля должна быть в диапазоне от %1 до %2"
                                                         ).arg(lengthSlider->minimum()).arg(lengthSlider->maximum()));
            //lengthLineEdit->setText(QString::number(lengthSlider->minimum())); // Установка минимального значения
        } else {
            lengthSlider->setValue(value); // Обновление значения слайдера
        }
    }
}

void MainWindow::onGenerateButtonClicked() {
    /**
     * @brief Обработчик события нажатия на кнопку "Сгенерировать".
     *
     * Проверяет, соответствует ли длина пароля ограничениям,
     * генерирует пароль с заданными параметрами и отображает результат.
     */
    if (lengthLineEdit->text().toInt() < lengthSlider->minimum() || lengthLineEdit->text().toInt() > lengthSlider->maximum()) {
        lengthLineEdit->setText(QString::number(lengthSlider->minimum()));
    }

    int length = lengthLineEdit->text().toInt();
    std::string password = generatePassword(length, uppercaseCheckbox->isChecked(), numbersCheckbox->isChecked(), symbolsCheckbox->isChecked());
    resultLabel->setText(password.c_str());
    resultWindow->show();
}

void MainWindow::onCopyButtonClicked() {
    /*
     * @brief Обработчик события нажатия на кнопку "Скопировать".
     *
     * Копирует сгенерированный пароль в буфер обмена
     * и закрывает окно результата.
     */
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(resultLabel->text());
    QMessageBox::information(resultWindow, "Успешно!", "Пароль скопирован в буфер обмена");
    resultWindow->close(); // Закрываем окно результата
}
std::string MainWindow::generatePassword(int length, bool useUppercase, bool useNumbers, bool useSymbols) {
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
     * @return Сгенерированный пароль/ключ в виде строки.
     */
    std::string characters = "abcdefghijklmnopqrstuvwxyz";
    if (useUppercase) characters += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (useNumbers) characters += "0123456789";
    if (useSymbols) characters += "!@#$%^&*()_+-=[]{};':\"\\|,.<>/?";

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(0, characters.size() - 1);

    std::string password;
    for (int i = 0; i < length; ++i) {
        password += characters[distribution(generator)];
    }

    return password;
}
