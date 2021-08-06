#include"workerManager.h"

WorkerManager::WorkerManager()
{
	//1、文件不存在
	ifstream ifs;
	ifs.open(FILENAME, ios::in);	//读文件

	if (!ifs.is_open())
	{
		//cout << "文件不存在" << endl;
		//初始化属性
		this->m_EmpNum = 0;
		this->m_EmpArray = NULL;

		this->m_FileIsEmpty = true;
		ifs.close();
		return;

	}

	//2、文件存在 但数据为空
	char ch;
	ifs >> ch;
	if (ifs.eof())
	{
		//文件为空
		//cout << "文件为空！" << endl;
		//初始化属性
		this->m_EmpNum = 0;
		this->m_EmpArray = NULL;

		this->m_FileIsEmpty = true;
		ifs.close();
		return;

	}

	//3、文件存在 且数据不为空
	int num = this->get_EmpNum();
	//cout << "职工人数为：" << num << endl;
	this->m_EmpNum = num;

	//开辟空间
	this->m_EmpArray = new Worker*[this->m_EmpNum];
	//将文件中数据存到数组中
	this->init_Emp();

	/*for (int i = 0; i < this->m_EmpNum; i++)
	{
		cout << "职工编号：" << this->m_EmpArray[i]->m_Id
			<< " 职工姓名：" << this->m_EmpArray[i]->m_Name
			<< " 部门编号：" << this->m_EmpArray[i]->m_DeptId << endl;
	}*/
	
}

//展示菜单
void WorkerManager::ShowMenu()
{
	cout << "***************************************" << endl;
	cout << "******* 欢迎使用职工管理系统！ ********" << endl;
	cout << "*********** 0.退出管理程序 ************" << endl;
	cout << "*********** 1.增加职工信息 ************" << endl;
	cout << "*********** 2.显示职工信息 ************" << endl;
	cout << "*********** 3.删除离职职工 ************" << endl;
	cout << "*********** 4.修改职工信息 ************" << endl;
	cout << "*********** 5.查找职工信息 ************" << endl;
	cout << "*********** 6.按照编号排序 ************" << endl;
	cout << "*********** 7.清空所有文档 ************" << endl;
	cout << "***************************************" << endl;
	cout << endl;


}

//退出功能
void WorkerManager::ExitSystem()
{
	cout << "欢迎下次使用" << endl;
	system("pause");
	exit(0);//退出程序

}

//添加职工
void WorkerManager::AddEmp()
{
	cout << "请输入添加职工数量：" << endl;

	int addNum = 0;
	cin >> addNum;

	if (addNum > 0)
	{
		//计算添加新的空间大小
		int newSize = this->m_EmpNum + addNum;	//新空间大小 = 原来记录人数 + 新增人数

		//开辟空间
		Worker** newSpace = new Worker*[newSize];

		//将原来空间下数据拷贝到新空间下
		if (this->m_EmpArray != NULL)
		{
			for (int i = 0; i < this->m_EmpNum; i++)
			{
				newSpace[i] = this->m_EmpArray[i];
			}
		}

		//批量添加新数据
		for (int i = 0; i < addNum; i++)
		{
			int id;
			string name;
			int dSelect;
			
			while (true)
			{
				cout << "请输入第 " << i + 1 << " 个新职工编号：" << endl;
				cin >> id;

				bool flag = true;

				for (int j = 0; j < this->m_EmpNum; j++)
				{
					if (this->m_EmpArray[j]->m_Id == id)
					{
						cout << "职工编号已存在，请重新输入！" << endl;
						flag = false;
						break;
					}
				}

				if (flag)
				{
					break;
				}
				
			}
			
			
			cout << "请输入第 " << i + 1 << " 个新职工姓名：" << endl;
			cin >> name;

			cout << "请选择第 " << i + 1 << " 个新职工岗位：" << endl;
			cout << "1、普通员工" << endl;
			cout << "2、经理" << endl;
			cout << "3、老板" << endl;
			cin >> dSelect;

			Worker* worker = NULL;
			switch (dSelect)
			{
			case 1:
				worker = new Employee(id, name, 1);
				break;
			case 2:
				worker = new Manager(id, name, 2);
				break;
			case 3:
				worker = new Boss(id, name, 3);
				break;
			default:
				break;
			}

			//将创建职工指针保存到数组中
			newSpace[this->m_EmpNum + i] = worker;

		}

		//释放原有空间
		delete[] this->m_EmpArray;

		//更改新空间指向
		this->m_EmpArray = newSpace;

		//更新职工人数
		this->m_EmpNum = newSize;

		//更新职工不为空
		this->m_FileIsEmpty = false;

		//成功添加后保存到文件中
		this->save();

		//提示添加成功
		cout << "成功添加" << addNum << "名新职工" << endl;

	}

	else
	{
		cout << "输入数据有误" << endl;
	}

	//按任意键后清屏回到上级目录
	system("pause");
	system("cls");

}

//保存文件
void WorkerManager::save()
{
	ofstream ofs;
	ofs.open(FILENAME, ios::out);	//用输出的方式打开文件-写文件

	//将每个人的数据写入到文件中
	for (int i = 0; i < this->m_EmpNum; i++)
	{
		ofs << this->m_EmpArray[i]->m_Id << " "
			<< this->m_EmpArray[i]->m_Name << " "
			<< this->m_EmpArray[i]->m_DeptId << endl;

	}

	//关闭文件
	ofs.close();

}

//统计文件中人数
int WorkerManager::get_EmpNum()
{
	ifstream ifs;
	ifs.open(FILENAME, ios::in);
	
	int id;
	string name;
	int dId;

	int num = 0;

	while (ifs >> id && ifs >> name && ifs >> dId)
	{
		//统计人数
		num++;
	}

	return num;

}

//初始化职工数组
void WorkerManager::init_Emp()
{
	ifstream ifs;
	ifs.open(FILENAME, ios::in);

	int id;
	string name;
	int dId;

	int index = 0;

	while (ifs >> id && ifs >> name && ifs >> dId)
	{
		Worker* worker = NULL;

		if (dId == 1)	//普通员工
		{
			worker = new Employee(id, name, dId);
		}
		else if (dId == 2)	//经理
		{
			worker = new Manager(id, name, dId);
		}
		else if(dId == 3)	//老板
		{
			worker = new Boss(id, name, dId);
		}

		this->m_EmpArray[index] = worker;
		index++;
	}

	ifs.close();

}

//显示职工
void WorkerManager::Show_Emp()
{
	//判断文件是否为空
	if (this->m_FileIsEmpty)
	{
		cout << "文件不存在或记录为空！" << endl;
	}
	else
	{
		for (int i = 0; i < this->m_EmpNum; i++)
		{
			//利用多态调用程序接口
			this->m_EmpArray[i]->ShowInfo();
		}
	}

	system("pause");
	system("cls");

}

//判断职工是否存在
int WorkerManager::IsExit(int id)
{
	int index = -1;

	for (int i = 0; i < this->m_EmpNum; i++)
	{
		if (this->m_EmpArray[i]->m_Id == id)
		{
			index = i;
			break;
		}
	}

	return index;
}

//删除职工
void WorkerManager::Del_Emp()
{
	if (this->m_FileIsEmpty)
	{
		cout << "文件不存在或记录为空！" << endl;
	}
	else
	{
		cout << "请输入删除职工编号：" << endl;
		int id = 0;
		cin >> id;

		int index = this->IsExit(id);

		if (index != -1)
		{
			//数据前移
			for (int i = index; i < this->m_EmpNum - 1; i++)
			{
				this->m_EmpArray[i] = this->m_EmpArray[i + 1];
			}

			//更新记录人员个数
			this->m_EmpNum--;

			//数据同步更新到文件
			this->save();

			cout << "删除成功！" << endl;
		}
		else
		{
			cout << "删除失败，未找到该职工！" << endl;
		}
	}

	system("pause");
	system("cls");

}

//修改职工
void WorkerManager::Mod_Emp()
{
	if (this->m_FileIsEmpty)
	{
		cout << "文件不存在或记录为空！" << endl;
	}
	else
	{
		cout << "请输入修改职工编号：" << endl;
		int id;
		cin >> id;

		int ret = this->IsExit(id);

		if (ret != -1)
		{
			delete this->m_EmpArray[ret];

			int newId = 0;
			string newName = "";
			int dSelect = 0;

			cout << "查找到" << id << "号职工，请输入新职工号：" << endl;
			cin >> newId;

			cout << "请输入新职工姓名：" << endl;
			cin >> newName;

			cout << "请输入岗位：" << endl;
			cout << "1、普通员工" << endl;
			cout << "2、经理" << endl;
			cout << "3、老板" << endl;
			cin >> dSelect;

			Worker* worker = NULL;

			switch (dSelect)
			{
			case 1:
				worker = new Employee(newId, newName, dSelect);
				break;
			case 2:
				worker = new Manager(newId, newName, dSelect);
				break;
			case 3:
				worker = new Boss(newId, newName, dSelect);
				break;
			default:
				break;
			}

			//更新数据到数组中
			this->m_EmpArray[ret] = worker;
			cout << "修改成功！" << endl;

			//保存到文件中
			this->save();
		}
		else
		{
			cout << "修改失败，查无此人" << endl;
		}
	}

	system("pause");
	system("cls");

}

//查找职工
void WorkerManager::Find_Emp()
{
	if (this->m_FileIsEmpty)
	{
		cout << "文件不存在或记录为空！" << endl;
	}
	else
	{
		cout << "请输入查找方式：" << endl;
		cout << "1、按职工编号查找" << endl;
		cout << "2、按职工姓名查找" << endl;
		
		int select = 0;
		cin >> select;

		if (select == 1)	//按编号查找
		{
			int id;
			cout << "请输入查找的职工编号：" << endl;
			cin >> id;

			int ret = IsExit(id);
			if (ret != -1)
			{
				cout << "查找成功！该职工信息如下：" << endl;
				this->m_EmpArray[ret]->ShowInfo();
			}
			else
			{
				cout << "查无此人" << endl;
			}
		}
		else if (select == 2)	//按姓名查找
		{
			string name;
			cout << "请输入查找的职工姓名：" << endl;
			cin >> name;

			//判断是否查到的标志
			bool flag = false;

			for (int i = 0; i < this->m_EmpNum; i++)
			{
				if (this->m_EmpArray[i]->m_Name == name)
				{
					cout << "查找成功！职工编号为" << this->m_EmpArray[i]->m_Id
						<< "号的职工信息如下：" << endl;

					this->m_EmpArray[i]->ShowInfo();

					flag = true;
				}
			}

			if (flag == false)
			{
				cout << "查无此人" << endl;
			}
		}
		else
		{
			cout << "输入选项有误" << endl;
		}
	}

	system("pause");
	system("cls");
}

//排序职工
void WorkerManager::Sort_Emp()
{
	if (this->m_FileIsEmpty)
	{
		cout << "文件不存在或记录为空！" << endl;

		system("pause");
		system("cls");
	}
	else
	{
		cout << "请选择排序的方式：" << endl;
		cout << "1、按职工编号升序排列" << endl;
		cout << "2、按职工编号降序排列" << endl;

		int select = 0;
		cin >> select;

		for (int i = 0; i < this->m_EmpNum; i++)	//选择排序
		{
			int minOrMax = i;	//声明最小值或最大值下标
			for (int j = i+1; j < this->m_EmpNum; j++)
			{
				if (select == 1)	//升序
				{
					if (this->m_EmpArray[minOrMax]->m_Id > this->m_EmpArray[j]->m_Id)
					{
						minOrMax = j;
					}
				}
				else  //降序
				{
					if (this->m_EmpArray[minOrMax]->m_Id < this->m_EmpArray[j]->m_Id)
					{
						minOrMax = j;
					}
				}
			}

			//判断一开始认定的最小值或最大值是不是计算的结果，如果不是则交换数据
			if (i != minOrMax)
			{
				Worker* temp = this->m_EmpArray[i];
				this->m_EmpArray[i] = this->m_EmpArray[minOrMax];
				this->m_EmpArray[minOrMax] = temp;
			}
			
		}

		cout << "排序成功！排序后的结果为：" << endl;
		this->save();
		this->Show_Emp();
	}
}

//清空文件
void WorkerManager::Clean_File()
{
	cout << "确定清空文件嘛？" << endl;
	cout << "1、确定" << endl;
	cout << "2、返回" << endl;

	int select = 0;
	cin >> select;

	if (select == 1)
	{
		ofstream ofs(FILENAME, ios::trunc);		//删除文件后再创建
		ofs.close();

		if (this->m_EmpArray != NULL)
		{
			//删除堆区的每个职工对象
			for (int i = 0; i < this->m_EmpNum; i++)
			{
				delete this->m_EmpArray[i];
				this->m_EmpArray[i] = NULL;
			}

			//删除堆区的数组指针
			delete[] this->m_EmpArray;
			this->m_EmpArray = NULL;
			this->m_EmpNum = 0;
			this->m_FileIsEmpty = true;
		}

		cout << "清空成功！" << endl;

	}

	system("pause");
	system("cls");
}

WorkerManager::~WorkerManager()
{
	if (this->m_EmpArray != NULL)
	{
		for (int i = 0; i < this->m_EmpNum; i++)
		{
			if (this->m_EmpArray[i] != NULL)
			{
				delete this->m_EmpArray[i];
			}
		}

		delete[] this->m_EmpArray;
		this->m_EmpArray = NULL;
	}
}