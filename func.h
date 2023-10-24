#pragma once
class CObj;
class CState;
class CFSM;

//�������� ����� �Լ��� �����صδ� ���

//�����Ͱ� ��ȿ���� �˷��ִ� �Լ�(��ȿ�� üũ)
bool IsValid(CObj*& _pTarget);

//������ ���� �ٲ�� �ȵǴϱ� const ���۷���
void SaveWString(const wstring& _str, FILE* _pFile);

//�ε��ؿ� ���� �˸°� �����ؾ��ϴϱ� �Ϲ� ���۷���
void LoadWString(wstring& _str, FILE* _pFile);

//������Ʈ�� ����
void Instantiate(CObj* _pNewObj, Vec2 _vPos, LAYER _eLayer);

//��ȭ�Լ�(� ���� 0�� 1 ���̷� �����ϴ� �Լ�)
void Saturate(float& _float);

//������ �ٲٴ� �Լ�
void ChangeLevel(LEVEL_TYPE _eNextLevel);

//�浹���� ���󺹱�(�÷��̾� / �� / ��ֹ� / ����)
void ResetCollision();

//��ư Ŭ���� ���
void BtnClickSound();


