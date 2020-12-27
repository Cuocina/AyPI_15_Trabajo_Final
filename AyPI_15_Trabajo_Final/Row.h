#ifndef ROW_H_
#define ROW_H_

namespace UGitRow {
	struct Row;
	Row* CreateRow();
	void AddToRow(Row* row, int item);
	int RemoveFromRow(Row* row);
	bool IsEmpty(const Row* row);
	void DestroyRow(Row* row);
}

#endif
