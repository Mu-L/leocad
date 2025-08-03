#pragma once

#include "lc_application.h"

#define LC_HTML_SINGLEPAGE    0x01
#define LC_HTML_INDEX         0x02
#define LC_HTML_LISTEND       0x08
#define LC_HTML_LISTSTEP      0x10
#define LC_HTML_SUBMODELS     0x40
#define LC_HTML_CURRENT_ONLY  0x80

class lcHTMLExportOptions
{
public:
	lcHTMLExportOptions(const Project* Project);
	void SaveDefaults();

	QString PathName;
	bool TransparentImages;
	bool SubModels;
	bool CurrentOnly;
	bool SinglePage;
	bool IndexPage;
	int StepImagesWidth;
	int StepImagesHeight;
	bool PartsListStep;
	bool PartsListEnd;
};

class Project
{
public:
	Project(bool IsPreview = false);
	~Project();

	Project(const Project&) = delete;
	Project(Project&&) = delete;
	Project& operator=(const Project&) = delete;
	Project& operator=(Project&&) = delete;

	const std::vector<std::unique_ptr<lcModel>>& GetModels() const
	{
		return mModels;
	}

	lcModel* GetModel(const QString& FileName) const;

	lcModel* GetActiveModel() const
	{
		return mActiveModel;
	}

	lcModel* GetMainModel() const
	{
		return !mModels.empty() ? mModels[0].get() : nullptr;
	}

	bool IsModified() const;
	void MarkAsModified();
	QString GetTitle() const;

	QString GetFileName() const
	{
		return mFileName;
	}

	QString GetImageFileName(bool AllowCurrentFolder) const;

	lcInstructions* GetInstructions();

	void SetActiveModel(lcModel* Model, bool UpdateInterface);
	void SetActiveModel(int ModelIndex, bool UpdateInterface);
	void SetActiveModel(const QString& FileName, bool UpdateInterface);

	lcModel* CreateNewModel(bool ShowModel);
	QString GetNewModelName(QWidget* ParentWidget, const QString& DialogTitle, const QString& CurrentName, const QStringList& ExistingModels) const;
	void ShowModelListDialog();

	bool Load(const QString& FileName, bool ShowErrors);
	bool Save(const QString& FileName);
	bool Save(QTextStream& Stream);
	void Merge(Project* Other);
	bool ImportLDD(const QString& FileName);
	bool ImportInventory(const QByteArray& Inventory, const QString& Name, const QString& Description);

	void SaveImage();
	bool ExportCurrentStep(const QString& FileName);
	bool ExportModel(const QString& FileName, lcModel* Model) const;
	bool Export3DStudio(const QString& FileName);
	void ExportBrickLink();
	bool ExportCOLLADA(const QString& FileName);
	bool ExportCSV(const QString& FileName);
	void ExportHTML(const lcHTMLExportOptions& Options);
	std::pair<bool, QString> ExportPOVRay(const QString& FileName);
	bool ExportWavefront(const QString& FileName);

	void UpdatePieceInfo(PieceInfo* Info) const;

protected:
	QString GetExportFileName(const QString& FileName, const QString& DefaultExtension, const QString& DialogTitle, const QString& DialogFilter) const;

	std::vector<lcModelPartsEntry> GetModelParts();
	void SetFileName(const QString& FileName);

	bool mIsPreview;
	bool mModified;
	QString mFileName;
	QFileSystemWatcher mFileWatcher;

	std::vector<std::unique_ptr<lcModel>> mModels;
	lcModel* mActiveModel;
	std::unique_ptr<lcInstructions> mInstructions;

	Q_DECLARE_TR_FUNCTIONS(Project);
};

inline lcModel* lcGetActiveModel()
{
	const Project* const Project = lcGetActiveProject();
	return Project ? Project->GetActiveModel() : nullptr;
}

