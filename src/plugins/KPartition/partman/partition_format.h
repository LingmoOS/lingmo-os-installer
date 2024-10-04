#ifndef PARTITION_FORMAT_H
#define PARTITION_FORMAT_H

#include <QObject>
#include "partition.h"

namespace LMInstaller {
namespace Partman {

bool MKfs(Partition::Ptr partition);

}
}
#endif // PARTITION_FORMAT_H
