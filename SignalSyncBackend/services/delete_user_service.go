package services

import (
	"database/sql"
	"fmt"
	"signalsync/backend/database"
)

// we can see that in every request we are having to "ValidateLogin", makes more sense to
// use something like JWT token, etc to validate login

func DeleteUserService(db *sql.DB, info *database.Login) bool {
	login_verification, _ := ValidateLoginService(db, info)
	if !login_verification {
		return false
	}
	_, err := db.Exec("DELETE FROM login WHERE username = ?", info.Username)
	fmt.Print(err)
	return err == nil
}
