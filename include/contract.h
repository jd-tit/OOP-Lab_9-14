//
// Created by jdt on 4/10/2022.
//

#ifndef LAB5_CONTRACT_DE_STUDII_CONTRACT_H
#define LAB5_CONTRACT_DE_STUDII_CONTRACT_H

#include "repo.h"
#include "domain.h"

class Contract : public Repo<Course>{
private:
    using Repo::Repo;

public:

    /**
     * Clear the Contract
     */
    void clear();

    /**
     * Save Contract to file
     * @param filename - name of file to put Contract in
     */
    void saveToFile(const std::string& filename);
};

#endif //LAB5_CONTRACT_DE_STUDII_CONTRACT_H
