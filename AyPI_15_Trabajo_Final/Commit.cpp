#include "Context.h"
#include "DateTime.h"
#include "User.h"
#include <string>
#include <ctime>
#include <random>
#include "Commit.h"
#include "CommitBag.h"

using UGit::Commit;
using namespace UContext;
using UDateTime::DateTime;
using UUser::User;
using std::string;
using UGit::CommitBag;

struct UGit::Commit {
	UGit::CommitBag* parents;
	string message;
	string hashCode;
	UDateTime::DateTime* context;
	UUser::User* user;
};

//Funciones auxiliares
bool RandomSecuencyInitialized = false;

string CreateHashCode() {
	string hashCode;
	char chars[] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','0','1','2','3','4','5','6','7','8','9' };
	if (!RandomSecuencyInitialized) {
		srand(time(NULL));
		RandomSecuencyInitialized = true;
	}
	while (hashCode.length() < 40) {
		hashCode += chars[rand() % 35];
	}
	return hashCode;
}

UGit::Commit* UGit::CreateCommit(void* parents, string message) {
	UGit::Commit* commit = new UGit::Commit;
	commit->parents = (CommitBag*)parents;
	commit->message = message;
	commit->hashCode = CreateHashCode();
	commit->context = UContext::GetNow();
	commit->user = UContext::GetCurrentUser();
	return commit;
}

UDateTime::DateTime* UGit::GetDate(const Commit* commit) {
	return commit->context;
}

UUser::User* UGit::GetAuthor(const Commit* commit) {
	return commit->user;
}

string UGit::GetMessage(const Commit* commit) {
	return commit->message;
}

void* UGit::GetParents(Commit* commit) {
	return commit->parents;
}

string UGit::GetHashCode(const Commit* commit) {
	return commit->hashCode;
}

string UGit::GetShortHashCode(const Commit* commit) {
	return commit->hashCode.substr(0, 7);
}

void UGit::DestroyCommit(Commit* commit) {
	if (commit != NULL) {
		DestroyDateTime(commit->context);
		delete commit;
	}
}
