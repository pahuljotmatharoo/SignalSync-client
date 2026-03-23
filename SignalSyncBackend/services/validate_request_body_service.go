package services

import (
	"fmt"
	"net/http"

	"github.com/gin-gonic/gin"
)

func ValidateRequestBodyService[T any](req *T, c *gin.Context) bool {
	if err := c.ShouldBind(req); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		fmt.Println(err)
		return false
	}
	return true
}
