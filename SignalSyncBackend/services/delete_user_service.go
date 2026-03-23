package services

import (
	"database/sql"
	"fmt"
	"signalsync/backend/database"
)

func DeleteUserService(db *sql.DB, info *database.Login) bool {
	login_verification, _ := ValidateLoginService(db, info)
	if !login_verification {
		return false
	}
	_, err := db.Exec("DELETE FROM login WHERE username = ?", info.Username)
	fmt.Print(err)
	return err == nil
}
