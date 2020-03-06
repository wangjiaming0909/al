#pragma once
#include <filesystem>
#include <vector>
#include <memory>
#include <unordered_map>

namespace filesync
{
using Entry = std::filesystem::directory_entry;

struct IDirObserver
{
	virtual void onUpdate() = 0;
};

struct IDirObservable
{
	virtual int subscribe(std::shared_ptr<IDirObserver> observer) = 0;
	virtual void unsubscribe(int id, std::shared_ptr<IDirObservable> observer) = 0;
};

class DirObservable : public IDirObservable
{
public:
	explicit DirObservable(const std::filesystem::path& path);

public:
	virtual int subscribe(std::shared_ptr<IDirObserver> observer) = 0;
	virtual void unsubscribe(int id, std::shared_ptr<IDirObservable> observer) = 0;
	void startObserve();

private:
	void cacheEntries();

private:
	std::filesystem::directory_entry dir_;
	std::vector<std::filesystem::directory_entry> entries_;

	std::unordered_map<int, std::weak_ptr<IDirObserver>> observers_;
};

}
