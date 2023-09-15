#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::MainWindow(Backend *backend, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_backend(backend)
{
    ui->setupUi(this);
    // execute backend function openPayday
    connect(ui->launchButton, &QPushButton::clicked, this, &MainWindow::onPushButtonClicked);
    // default options
    ui->epicMMButton->setChecked(true);
    ui->normalButton->setChecked(true);
    connect(m_backend, &Backend::configReady, [this](const QJsonObject &result)
            {
        for (const QString &key : result.keys())
        {
            if (key == "mm")
            {
                if (result.value(key) == "steam")
                {
                    ui->steamMMButton->setChecked(true);
                }
                else
                {
                    ui->epicMMButton->setChecked(true);
                }
            }
            else if (key == "profile")
            {
                if (result.value(key) == "rush")
                {
                    ui->rushButton->setChecked(true);
                }
                else
                {
                    ui->normalButton->setChecked(true);
                }
            }
        } });
    setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    backend->loadConfig();

    // for every button in groupButton
    for (int i = 0; i < ui->buttonGroup->buttons().size(); i++)
    {
        // printf("Button %d\n", ui->buttonGroup->buttons().at(i)->objectName().toInt());
        // qDebug() << ui->buttonGroup->buttons().at(i)->objectName().toInt();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onPushButtonClicked()
{
    m_backend->deleteAllFolder("../mods");
    m_backend->deleteAllFolder("../assets/mod_overrides");
    std::string args = " -skip_intro";
    QJsonObject config;
    for (int i = 0; i < ui->groupBoxMM->children().size(); i++)
    {
        // if it's a radio button
        if (ui->groupBoxMM->children().at(i)->metaObject()->className() == QString("QRadioButton"))
        {
            // cast it to a radio button
            QRadioButton *radioButton = qobject_cast<QRadioButton *>(ui->groupBoxMM->children().at(i));
            // if it's checked
            if (radioButton->isChecked() && radioButton->objectName() == "steamMMButton")
            {
                // add -steammm to args
                args += " -steamMM";
                config.insert("mm", "steam");
                qDebug() << "steam";
            }
            else if (radioButton->isChecked() && radioButton->objectName() == "epicMMButton")
            {
                config.insert("mm", "epic");
            }
        }
    }
    QString profileFolder;
    QString overrideFolder;
    for (int i = 0; i < ui->groupBoxProfile->children().size(); i++)
    {
        // if it's a radio button
        if (ui->groupBoxProfile->children().at(i)->metaObject()->className() == QString("QRadioButton"))
        {
            if (ui->groupBoxProfile->children().at(i)->objectName() == "rushButton")
            {
                // cast it to a radio button
                QRadioButton *radioButton = qobject_cast<QRadioButton *>(ui->groupBoxProfile->children().at(i));
                // if it's checked
                if (radioButton->isChecked())
                {
                    profileFolder = "mods_rush";
                    overrideFolder = "mod_overrides_rush";
                    config.insert("profile", "rush");
                }
            }
            else if (ui->groupBoxProfile->children().at(i)->objectName() == "normalButton")
            {
                // cast it to a radio button
                QRadioButton *radioButton = qobject_cast<QRadioButton *>(ui->groupBoxProfile->children().at(i));
                // if it's checked
                if (radioButton->isChecked())
                {
                    profileFolder = "mods_normal";
                    overrideFolder = "mod_overrides_normal";
                    config.insert("profile", "normal");
                }
            }
        }
    }
    m_backend->moveAllFiles(profileFolder, "../mods");
    m_backend->moveAllFiles(overrideFolder, "../assets/mod_overrides");
    m_backend->startExe("C:\\Program Files (x86)\\Steam\\steamapps\\common\\PAYDAY 2", "payday2_win32_release.exe", args.c_str());
    m_backend->saveConfig(config);
    // close window
    close();
}