#include "HistoryManager.h"

void HistoryManager::addRecord(const HistoryRecord& record)
{
    history.push_back(record);
}

const std::vector<HistoryRecord>& HistoryManager::getRecords() const
{
    return history;
}
