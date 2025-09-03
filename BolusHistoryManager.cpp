#include "BolusHistoryManager.h"

void BolusHistoryManager::addRecord(const BolusRecord &record)
{
    history.push_back(record);
}

const std::vector<BolusRecord>& BolusHistoryManager::getRecords() const
{
    return history;
}
