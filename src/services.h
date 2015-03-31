#ifndef BLUEZQT_SERVICES_H
#define BLUEZQT_SERVICES_H

#include <QString>

namespace BluezQt
{

namespace Services
{

const QString SerialPort = QLatin1String("00001101-0000-1000-8000-00805F9B34FB");
const QString DialupNetworking = QLatin1String("00001103-0000-1000-8000-00805F9B34FB");
const QString ObexObjectPush = QLatin1String("00001105-0000-1000-8000-00805F9B34FB");
const QString ObexFileTransfer = QLatin1String("00001106-0000-1000-8000-00805F9B34FB");
const QString AudioSource = QLatin1String("0000110A-0000-1000-8000-00805F9B34FB");
const QString AudioVideoRemoteControlTarget = QLatin1String("0000110C-0000-1000-8000-00805F9B34FB");
const QString AdvancedAudioDistribution = QLatin1String("0000110D-0000-1000-8000-00805F9B34FB");
const QString AudioVideoRemoteControl = QLatin1String("0000110E-0000-1000-8000-00805F9B34FB");
const QString HeadsetAudioGateway = QLatin1String("00001112-0000-1000-8000-00805F9B34FB");
const QString Nap = QLatin1String("00001116-0000-1000-8000-00805F9B34FB");
const QString HandsfreeAudioGateway = QLatin1String("0000111F-0000-1000-8000-00805F9B34FB");
const QString HumanInterfaceDevice = QLatin1String("00001124-0000-1000-8000-00805F9B34FB");
const QString SimAccess = QLatin1String("0000112D-0000-1000-8000-00805F9B34FB");
const QString PhonebookAccessPse = QLatin1String("0000112F-0000-1000-8000-00805F9B34FB");
const QString MessageAccessServer = QLatin1String("00001132-0000-1000-8000-00805F9B34FB");
const QString PnpInformation = QLatin1String("00001200-0000-1000-8000-00805F9B34FB");

}

} // namespace BluezQt

#endif // BLUEZQT_SERVICES_H
