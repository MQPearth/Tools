#include "configmanager.h"

#include <QStandardPaths>
#include <QFile>
#include <QJsonDocument>
#include <QDir>
#include <QNetworkAccessManager>
#include <QApplication>

QString ConfigManager::default_notify_url = "192.168.1.60:19991";

ConfigManager::ConfigManager()
{
	m_config = QJsonObject();
}

ConfigManager& ConfigManager::instance()
{
	static ConfigManager inst;
	return inst;
}

QString ConfigManager::configFilePath() const
{
	QString configDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/timershutdown";
	QDir().mkpath(configDir);
	return configDir + "/config.json";
}

bool ConfigManager::loadConfig()
{
    ConfigManager& self = instance();
    QString path = self.configFilePath();
    QFile file(path);

    if (!file.exists()) {
        // 文件不存在，首次使用
        self.m_config = QJsonObject();
        self.m_notify_url = ConfigManager::default_notify_url;

        self.m_config["notifyUrl"] = ConfigManager::default_notify_url;

        // 创建文件并写入默认值
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QJsonDocument doc(self.m_config);
            file.write(doc.toJson(QJsonDocument::Indented));
            file.close();
            return true;
        }
        // 无法写入
        return false;
    }

    // 文件存在，尝试读取
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError || !doc.isObject()) {
        return false;
    }

    self.m_config = doc.object();
    self.m_notify_url = self.m_config["notifyUrl"].toString();
    return true;
}

bool ConfigManager::saveConfig()
{
    ConfigManager& self = instance();

    QFile file(self.configFilePath());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QJsonDocument doc(self.m_config);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}


QString ConfigManager::getNotifyUrl() {

    return instance().m_notify_url;
}

void ConfigManager::setNotifyUrl(QString notifyUrl) {
    ConfigManager& self = instance();
    self.m_notify_url = notifyUrl;
    self.m_config["notifyUrl"] = notifyUrl;
}

QJsonObject& ConfigManager::jsonConfig()
{
	return instance().m_config;
}

const QJsonObject& ConfigManager::jsonConfigConst()
{
	return instance().m_config;
}


