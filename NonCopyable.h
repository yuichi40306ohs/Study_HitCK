#pragma once

class NonCopyable {
public:
    NonCopyable() = default; // �f�t�H���g�R���X�g���N�^

    // �R�s�[�R���X�g���N�^���폜
    NonCopyable(const NonCopyable&) = delete;

    // ������Z�q���폜�i�R�s�[�����S�ɋ֎~����ꍇ�j
    NonCopyable& operator=(const NonCopyable&) = delete;
};