#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QJsonObject>
#include <QNetworkAccessManager>


class ConfigManager : public QObject
{
	Q_OBJECT

public:
	static bool loadConfig();

	static bool saveConfig();

	static void setNotifyUrl(QString notifyUrl);

	static QString getNotifyUrl();

	static QJsonObject& jsonConfig();

	static const QJsonObject& jsonConfigConst();

private:

	static ConfigManager& instance();

	static QString default_notify_url;

	ConfigManager();

	QString configFilePath() const;

	QJsonObject m_config;

	QString m_notify_url;
};

#endif
