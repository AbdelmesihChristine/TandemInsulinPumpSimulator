#ifndef BOLUSHISTORYMANAGER_H
#define BOLUSHISTORYMANAGER_H

#include <vector>
#include "BolusRecord.h"

class BolusHistoryManager
{
public:
    void addRecord(const BolusRecord& record);
    const std::vector<BolusRecord>& getRecords() const;

private:
    std::vector<BolusRecord> history;
};

#endif // BOLUSHISTORYMANAGER_H
